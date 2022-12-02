#include "index/index.h"
#include "json/json.h"

#include <minty/core/search/search.h>

namespace minty::core {
    search_engine::search_engine(
        std::string_view ns,
        std::string_view host,
        std::string_view api_key
    ) :
        client(host, api_key),
        post_index(
            this,
            ns,
            "post",
            search::config::post
        ),
        tag_index(
            this,
            ns,
            "tag",
            search::config::tag
        )
    {}

    auto search_engine::add_post(const post_search& post) -> ext::task<> {
        co_await client
            .create_doc(
                post_index,
                post.id,
                json(post).dump()
            )
            .send();
    }

    auto search_engine::add_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        co_await client
            .update_doc(
                post_index,
                post_id,
                json({
                    {"script", {
                        {"lang", "painless"},
                        {"params", {
                            {"tag", tag_id}
                        }},
                        {"source",
                            "if (!ctx._source.tags.contains(params.tag)) {"
                                "ctx._source.tags.add(params.tag);"
                            "}"
                        }
                    }}
                }).dump()
            )
            .send();
    }

    auto search_engine::add_posts(
        std::span<const post_search> posts
    ) -> ext::task<std::vector<std::string>> {
        return bulk_index(post_index, posts);
    }

    auto search_engine::add_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<> {
        co_await client
            .update_doc(
                tag_index,
                tag_id,
                json({
                    {"script", {
                        {"lang", "painless"},
                        {"params", {
                            {"alias", alias}
                        }},
                        {"source",
                            "if (!ctx._source.names.contains(params.alias)) {"
                                "ctx._source.names.add(params.alias);"
                            "}"
                        }
                    }},
                    {"upsert", {
                        {"names", {alias}}
                    }}
                }).dump()
            )
            .send();
    }

    auto search_engine::add_tags(
        std::span<const tag_text> tags
    ) -> ext::task<std::vector<std::string>> {
        return bulk_index(tag_index, tags);
    }

    template <typename T>
    auto search_engine::bulk_index(
        std::string_view index,
        std::span<const T> documents
    ) -> ext::task<std::vector<std::string>> {
        namespace ranges = std::ranges;

        using action = elastic::bulk::action;
        using create = elastic::bulk::create;

        TIMBER_FUNC();
        TIMBER_DEBUG("bulk {} {} documents", create::name, documents.size());

        if (documents.empty()) co_return std::vector<std::string>();

        auto actions = std::vector<action>();
        actions.reserve(documents.size());

        ranges::transform(
            documents,
            std::back_inserter(actions),
            [](const auto& document) {
                return create(
                    { .id = std::string(document.id) },
                    json(document).dump()
                );
            }
        );

        auto response = co_await client
            .bulk(index, actions)
            .filter_path({"items.*.error"})
            .send();

        auto errors = std::vector<std::string>();

        auto items = response.find("items");
        if (items != response.end()) {
            for (auto& item : *items) {
                auto error = item[create::name]["error"];
                errors.emplace_back(error["reason"].template get<std::string>());
            }
        }

        co_return errors;
    }

    auto search_engine::create_indices() -> ext::task<> {
        for (auto* idx : {
            &post_index,
            &tag_index
        }) co_await idx->create();
    }

    auto search_engine::delete_indices(
        std::initializer_list<std::string_view> index
    ) -> ext::task<> {
        co_await client
            .delete_index(index)
            .ignore_unavailable(true)
            .send();
    }

    auto search_engine::delete_indices() -> ext::task<> {
        return delete_indices({
            post_index,
            tag_index
        });
     }

    auto search_engine::delete_post(const UUID::uuid& post_id) -> ext::task<> {
        co_await client.delete_doc(post_index, post_id).send();
    }

    auto search_engine::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<> {
        co_await client
            .update_doc(
                tag_index,
                tag_id,
                json({
                    {"script", {
                        {"lang", "painless"},
                        {"params", {
                            {"alias", alias}
                        }},
                        {"source",
                            "if (ctx._source.names.contains(params.alias)) {"
                                "ctx._source.names.remove("
                                    "ctx._source.names.indexOf(params.alias)"
                                ");"
                            "}"
                        }
                    }}
                }).dump()
            )
            .send();
    }

    auto search_engine::delete_tag(const UUID::uuid& tag_id) -> ext::task<> {
        co_await client.delete_doc(tag_index, tag_id).send();
    }

    auto search_engine::find_posts(
        const post_query& query
    ) -> ext::task<search_result<UUID::uuid>> {
        using minty::core::sort_order;
        using minty::core::post_sort_value;

        auto result = json({
            {"_source", false},
            {"from", query.from},
            {"size", query.size}
        });

        if (query.text || !query.tags.empty()) {
            result["query"] = {
                {"bool", {}}
            };

            auto& b = result["query"]["bool"];

            if (query.text) {
                b["must"] = json({
                    {"multi_match", {
                        {"query", query.text.value()},
                        {"fields", {
                            "title^3",
                            "description"
                        }}
                    }}
                });
            }

            if (!query.tags.empty()) {
                b["filter"] = json({
                    {"terms_set", {
                        {"tags", {
                            {"terms", query.tags},
                            {"minimum_should_match_script", {
                                {"source", std::to_string(query.tags.size())}
                            }}
                        }}
                    }}
                });
            }
        }

        auto value = std::string_view();
        switch (query.sort.value) {
            case post_sort_value::date_created: value = "created"; break;
            case post_sort_value::date_modified: value = "modified"; break;
            case post_sort_value::relevance: value = "_score"; break;
            case post_sort_value::title: value = "title.keyword"; break;
        }

        auto order = std::string_view();
        switch (query.sort.order) {
            case sort_order::ascending: order = "asc"; break;
            case sort_order::descending: order = "desc"; break;
        }

        result["sort"] = {
            {value, order}
        };

        co_return co_await search_ids(post_index, result.dump());
    }

    auto search_engine::find_tags(
        const tag_query& query
    ) -> ext::task<search_result<UUID::uuid>> {
        co_return co_await search_ids(tag_index, json({
            {"_source", false},
            {"from", query.from},
            {"size", query.size},
            {"query", {
                {"bool", {
                    {"must", {
                        {"multi_match", {
                            {"query", query.name},
                            {"type", "bool_prefix"},
                            {"fields", {
                                "names",
                                "names._2gram",
                                "names._3gram"
                            }}
                        }}
                    }},
                    {"must_not", {
                        {"ids", {
                            {"values", query.exclude}
                        }}
                    }}
                }}
            }}
        }).dump());
    }

    auto search_engine::remove_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        co_await client
            .update_doc(
                post_index,
                post_id,
                json({
                    {"script", {
                        {"lang", "painless"},
                        {"params", {
                            {"tag", tag_id}
                        }},
                        {"source",
                            "if (ctx._source.tags.contains(params.tag)) {"
                                "ctx._source.tags.remove("
                                    "ctx._source.tags.indexOf(params.tag)"
                                ");"
                            "}"
                        }
                    }}
                }).dump()
            )
            .send();
    }

    auto search_engine::search(
        std::string_view index,
        std::string_view query
    ) -> ext::task<elastic::json> {
        TIMBER_FUNC();

        try {
           co_return co_await client.search(index, query).send();
        }
        catch (const elastic::es_error& ex) {
            TIMBER_ERROR(
                "{} search ({}): {}",
                index,
                ex.status(),
                ex.what()
            );

            throw minty::minty_error("internal search error");
        }
    }

    auto search_engine::search_ids(
        std::string_view index,
        std::string_view query
    ) -> ext::task<search_result<UUID::uuid>> {
        auto response = co_await search(index, query);
        auto result = search_result<UUID::uuid>();

        response["hits"]["total"]["value"].get_to(result.total);

        for (auto& hit : response["hits"]["hits"]) {
            const auto id = hit["_id"].get<std::string>();
            TIMBER_DEBUG("search results: {}", id);

            result.hits.emplace_back(id);
        }

        TIMBER_DEBUG(
            "search results: {} hits out of {} total",
            result.hits.size(),
            result.total
        );

        co_return result;
    }

    auto search_engine::update_post_date_modified(
        const UUID::uuid& post_id,
        decltype(post::date_modified) date_modified
    ) -> ext::task<> {
        co_await client
            .update_doc(
                post_index,
                post_id,
                json({
                    {"doc", {
                        {"modified", date_modified}
                    }}
                }).dump()
            )
            .send();
    }

    auto search_engine::update_post_description(
        const post_update& post
    ) -> ext::task<> {
        co_await client
            .update_doc(
                post_index,
                post.id,
                json({
                    {"doc", {
                        {"description", post.new_data},
                        {"modified", post.date_modified}
                    }}
                }).dump()
            )
            .send();
    }

    auto search_engine::update_post_title(
        const post_update& post
    ) -> ext::task<> {
        co_await client
            .update_doc(
                post_index,
                post.id,
                json({
                    {"doc", {
                        {"title", post.new_data},
                        {"modified", post.date_modified}
                    }}
                }).dump()
            )
            .send();
    }

    auto search_engine::update_tag_name(
        const UUID::uuid& tag_id,
        std::string_view old_name,
        std::string_view new_name
    ) -> ext::task<> {
        co_await client
            .update_doc(
                tag_index,
                tag_id,
                json({
                    {"script", {
                        {"lang", "painless"},
                        {"params", {
                            {"old", old_name},
                            {"new", new_name}
                        }},
                        {"source",
                            "if (ctx._source.names.contains(params.old)) {"
                                "ctx._source.names.remove("
                                    "ctx._source.names.indexOf(params.old)"
                                ");"
                                "ctx._source.names.add(params.new)"
                            "}"
                        }
                    }}
                }).dump()
            )
            .send();

        TIMBER_DEBUG(
            "Update tag name ({}): '{}' -> '{}'",
            tag_id,
            old_name,
            new_name
        );
    }
}
