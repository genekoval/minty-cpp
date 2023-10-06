#include <internal/server/http/extractor.hpp>
#include <internal/server/http/parser.hpp>
#include <internal/server/http/server.hpp>

namespace minty::server::http {
    using ::http::server::del;
    using ::http::server::get;
    using ::http::server::post;
    using ::http::server::put;
    using ::http::server::extractor::header;
    using ::http::server::extractor::path;
    using ::http::server::extractor::query;
    using ::http::server::extractor::stream;

    auto router(const server_info& info, core::repo& repo)
        -> ::http::server::router {
        auto paths = ::http::server::path();

        paths.insert("/", get([&info]() { return info; }));

        paths.insert(
            "/comment/:id",
            get([&repo](path<"id", UUID::uuid> comment_id
                ) -> ext::task<std::optional<comment>> {
                co_return co_await repo.get_comment(comment_id);
            })
                .post(
                    [&repo](
                        path<"id", UUID::uuid> parent_id,
                        std::string content
                    ) -> ext::task<comment_data> {
                        co_return co_await repo.add_reply(parent_id, content);
                    }
                )
                .put(
                    [&repo](
                        path<"id", UUID::uuid> comment_id,
                        std::string content
                    ) -> ext::task<std::string> {
                        co_return co_await repo.set_comment_content(
                            comment_id,
                            content
                        );
                    }
                )
                .del(
                    [&repo](
                        path<"id", UUID::uuid> comment_id,
                        query<"r", std::optional<bool>> recursive
                    ) -> ext::task<int> {
                        co_return co_await repo.delete_comment(
                            comment_id,
                            recursive->value_or(false)
                        )
                            ? 204
                            : 404;
                    }
                )
        );

        paths.insert(
            "/comments/:post",
            get([&repo](path<"post", UUID::uuid> post_id
                ) -> ext::task<comment_tree> {
                co_return co_await repo.get_comments(post_id);
            })
                .post(
                    [&repo](
                        path<"post", UUID::uuid> post_id,
                        std::string content
                    ) -> ext::task<comment_data> {
                        co_return co_await repo.add_comment(post_id, content);
                    }
                )
        );

        paths.insert(
            "/object",
            post(
                [&repo](
                    header<"content-length", std::size_t> content_length,
                    stream stream
                ) -> ext::task<object_preview> {
                    co_return co_await repo.add_object_data(
                        content_length,
                        [&](fstore::part& part) -> ext::task<> {
                            std::size_t written = 0;

                            while (written < content_length) {
                                const auto chunk = co_await stream.read();
                                if (chunk.empty()) throw netcore::eof();

                                co_await part.write(chunk);
                                written += chunk.size();
                            }

                            const auto chunk = co_await stream.read();

                            if (!chunk.empty()) {
                                throw ::http::error_code(
                                    400,
                                    "Request body larger than expected"
                                );
                            }
                        }
                    );
                }
            )
        );

        paths.insert(
            "/object/url",
            post(
                [&repo](std::string url
                ) -> ext::task<std::vector<object_preview>> {
                    co_return co_await repo.add_objects_url(url);
                }
            )
        );

        paths.insert(
            "/object/:object",
            get([&repo](path<"object", UUID::uuid> object_id
                ) -> ext::task<std::optional<object>> {
                co_return co_await repo.get_object(object_id);
            })
        );

        paths.insert("/post", post([&repo]() -> ext::task<UUID::uuid> {
                         co_return co_await repo.create_post_draft();
                     }));

        paths.insert(
            "/post/:post",
            get([&repo](path<"post", UUID::uuid> post_id
                ) -> ext::task<std::optional<minty::post>> {
                co_return co_await repo.get_post(post_id);
            })
                .put([&repo](path<"post", UUID::uuid> draft_id) -> ext::task<> {
                    co_await repo.create_post(draft_id);
                })
                .del([&repo](path<"post", UUID::uuid> post_id) -> ext::task<> {
                    co_await repo.delete_post(post_id);
                })
        );

        paths.insert(
            "/post/:post/title",
            put([&repo](path<"post", UUID::uuid> post_id, std::string title)
                    -> ext::task<modification<std::optional<std::string>>> {
                co_return co_await repo.set_post_title(post_id, title);
            })
        );

        paths.insert(
            "/post/:post/description",
            put([&repo](
                    path<"post", UUID::uuid> post_id,
                    std::string description
                ) -> ext::task<modification<std::optional<std::string>>> {
                co_return co_await repo.set_post_description(
                    post_id,
                    description
                );
            })
        );

        paths.insert(
            "/post/:post/related/:related",
            put([&repo](
                    path<"post", UUID::uuid> post_id,
                    path<"related", UUID::uuid> related_id
                ) -> ext::task<> {
                co_await repo.add_related_post(post_id, related_id);
            })
                .del(
                    [&repo](
                        path<"post", UUID::uuid> post_id,
                        path<"related", UUID::uuid> related
                    ) -> ext::task<> {
                        co_await repo.delete_related_post(post_id, related);
                    }
                )
        );

        paths.insert(
            "/post/:post/tag/:tag",
            put([&repo](
                    path<"post", UUID::uuid> post_id,
                    path<"tag", UUID::uuid> tag_id
                ) -> ext::task<> {
                co_await repo.add_post_tag(post_id, tag_id);
            })
                .del(
                    [&repo](
                        path<"post", UUID::uuid> post_id,
                        path<"tag", UUID::uuid> tag_id
                    ) -> ext::task<> {
                        co_await repo.delete_post_tag(post_id, tag_id);
                    }
                )
        );

        paths.insert(
            "/post/:post/objects",
            post(
                [&repo](
                    path<"post", UUID::uuid> post_id,
                    std::vector<UUID::uuid> objects
                ) -> ext::task<time_point> {
                    co_return co_await repo
                        .add_post_objects(post_id, objects, {});
                }
            )
                .del(
                    [&repo](
                        path<"post", UUID::uuid> post_id,
                        std::vector<UUID::uuid> objects
                    ) -> ext::task<time_point> {
                        co_return co_await repo.delete_post_objects(
                            post_id,
                            objects
                        );
                    }
                )
        );

        paths.insert(
            "/post/:post/objects/:destination",
            post(
                [&repo](
                    path<"post", UUID::uuid> post_id,
                    path<"destination", UUID::uuid> destination,
                    std::vector<UUID::uuid> objects
                ) -> ext::task<time_point> {
                    co_return co_await repo
                        .add_post_objects(post_id, objects, destination);
                }
            )
        );

        paths.insert(
            "/posts",
            get([&repo](
                    query<"from", std::optional<std::uint32_t>> from,
                    query<"size", std::optional<std::uint32_t>> size,
                    query<"q", std::optional<std::string_view>> text,
                    query<"tags", std::optional<std::vector<UUID::uuid>>> tags,
                    query<"vis", std::optional<visibility>> visibility,
                    query<"sort", std::optional<post_sort_value>> sort,
                    query<"order", std::optional<sort_order>> order
                ) -> ext::task<search_result<post_preview>> {
                const auto sort_value =
                    sort.value.value_or(post_sort_value::date_created);

                co_return co_await repo.get_posts(post_query {
                    .from = from->value_or(0),
                    .size = size->value_or(10),
                    .text = text.value ? std::optional<std::string>(*text)
                                       : std::optional<std::string>(),
                    .tags =
                        tags.value ? *tags.value : std::vector<UUID::uuid>(),
                    .visibility = visibility.value.value_or(visibility::pub),
                    .sort = post_sort(
                        sort_value,
                        order->value_or(default_sort_order(sort_value))
                    )});
            })
        );

        paths.insert(
            "/tag/:tag",
            get([&repo](path<"tag", UUID::uuid> tag_id
                ) -> ext::task<std::optional<tag>> {
                co_return co_await repo.get_tag(tag_id);
            })
                .post([&repo](path<"tag"> name) -> ext::task<UUID::uuid> {
                    co_return co_await repo.add_tag(name);
                })
                .del([&repo](path<"tag", UUID::uuid> tag_id) -> ext::task<> {
                    co_await repo.delete_tag(tag_id);
                })
        );

        paths.insert(
            "/tag/:tag/name/:name",
            put([&repo](
                    path<"tag", UUID::uuid> tag_id,
                    path<"name"> new_name,
                    query<"main", std::optional<bool>> main
                ) -> ext::task<tag_name> {
                if (main->has_value() && *main) {
                    co_return co_await repo.set_tag_name(tag_id, new_name);
                }

                co_return co_await repo.add_tag_alias(tag_id, new_name);
            })
                .del(
                    [&repo](path<"tag", UUID::uuid> tag_id, path<"name"> alias)
                        -> ext::task<tag_name> {
                        co_return co_await repo.delete_tag_alias(tag_id, alias);
                    }
                )
        );

        paths.insert(
            "/tag/:tag/description",
            put([&repo](path<"tag", UUID::uuid> tag_id, std::string description)
                    -> ext::task<std::optional<std::string>> {
                co_return co_await repo.set_tag_description(
                    tag_id,
                    description
                );
            })
        );

        paths.insert(
            "/tag/:tag/source",
            post(
                [&repo](path<"tag", UUID::uuid> tag_id, std::string url)
                    -> ext::task<source> {
                    co_return co_await repo.add_tag_source(tag_id, url);
                }
            )
        );

        paths.insert(
            "/tag/:tag/source/:source",
            del([&repo](
                    path<"tag", UUID::uuid> tag_id,
                    path<"source", std::int64_t> source_id
                ) -> ext::task<> {
                co_await repo.delete_tag_source(tag_id, source_id);
            })
        );

        paths.insert(
            "/tags",
            get([&repo](
                    query<"from", std::optional<std::uint32_t>> from,
                    query<"size", std::optional<std::uint32_t>> size,
                    query<"name", std::string> name,
                    query<"exclude", std::optional<std::vector<UUID::uuid>>>
                        exclude
                ) -> ext::task<search_result<tag_preview>> {
                co_return co_await repo.get_tags(tag_query {
                    .from = from->value_or(0),
                    .size = size->value_or(10),
                    .name = name,
                    .exclude = exclude.value ? *exclude.value
                                             : std::vector<UUID::uuid>()});
            })
        );

        TIMBER_DEBUG("Routes:\n{}", paths.to_string());

        return paths;
    }
}
