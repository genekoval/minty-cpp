#include "utility.hpp"

#include <internal/core/api.hpp>
#include <internal/core/comment_tree.hpp>
#include <internal/core/preview.hpp>

#include <threadpool/threadpool>
#include <uri/uri>

namespace minty::core {
    api::api(
        repo::db::database& db,
        object_store& objects,
        downloader& dl,
        search_engine& search
    ) :
        db(&db),
        objects(&objects),
        dl(&dl),
        search(&search)
    {}

    auto api::add_comment(
        const UUID::uuid& post_id,
        std::string_view content
    ) -> comment_data {
        TIMBER_FUNC();

        const auto comment = db->create_comment(
            post_id,
            format_comment(content)
        );

        return {
            comment.id,
            comment.content,
            comment.indent,
            comment.date_created
        };
    }

    auto api::add_object(
        bucket& bucket,
        fstore::object&& object,
        const std::optional<std::string>& src
    ) -> ext::task<object_preview> {
        auto preview_id = std::optional<UUID::uuid>();
        auto error = std::optional<std::string>();

        try {
            preview_id = co_await generate_preview(bucket, object);
        }
        catch (const std::exception& ex) {
            error = ex.what();
        }

        db->create_object(object.id, preview_id, src);
        if (error) db->create_object_preview_error(object.id, *error);

        co_return object_preview {
            std::move(object.id),
            std::move(preview_id),
            std::move(object.type),
            std::move(object.subtype)
        };
    }

    auto api::add_objects_url(
        std::string_view url
    ) -> ext::task<std::vector<object_preview>> {
        TIMBER_FUNC();

        auto bucket = co_await objects->connect();
        auto objects = std::vector<fstore::object>();

        const auto used_scraper = co_await dl->fetch(url, [&](
            auto& file
        ) -> ext::task<> {
            objects.push_back(co_await bucket.add(
                {},
                co_await file.size(),
                [&file](auto&& part) -> ext::task<> {
                    co_await file.read([&part](auto&& chunk) -> ext::task<> {
                        co_await part.write(chunk);
                    });
                }
            ));
        });

        const auto src = used_scraper ?
            (co_await add_source(url)).id : std::optional<std::string>();

        auto result = std::vector<object_preview>();

        for (auto&& obj : objects) {
            result.push_back(co_await add_object(bucket, std::move(obj), src));
        }

        co_return result;
    }

    auto api::add_post(post_parts parts) -> ext::task<UUID::uuid> {
        TIMBER_FUNC();

        const auto result = db->create_post(
            format_title(parts.title),
            format_description(parts.description),
            parts.objects,
            parts.tags
        );

        try {
            co_await search->add_post(result);
        }
        catch (const std::runtime_error& ex) {
            TIMBER_ERROR("Failed to index post ({}): {}", result.id, ex.what());
        }

        co_return result.id;
    }

    auto api::add_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        std::int16_t position
    ) -> ext::task<decltype(post::date_modified)> {
        TIMBER_FUNC();

        const auto date_modified =
            db->create_post_objects(post_id, objects, position);

        co_await search->update_post_date_modified(post_id, date_modified);

        co_return date_modified;
    }

    auto api::add_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        TIMBER_FUNC();

        db->create_post_tag(post_id, tag_id);
        co_await search->add_post_tag(post_id, tag_id);
    }

    auto api::add_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> void {
        TIMBER_FUNC();

        db->create_related_post(post_id, related);
    }

    auto api::add_reply(
        const UUID::uuid& parent_id,
        std::string_view content
    ) -> comment_data {
        TIMBER_FUNC();

        const auto comment = db->create_reply(
            parent_id,
            format_comment(content)
        );

        return {
            comment.id,
            comment.content,
            comment.indent,
            comment.date_created
        };
    }

    auto api::add_source(std::string_view url) -> ext::task<source> {
        constexpr auto host_prefix = std::string_view("www.");

        const auto uri = uri::uri(std::string(url));

        const auto scheme = uri.scheme();
        auto host = uri.host();
        if (host.starts_with(host_prefix)) {
            host = host.substr(host_prefix.size());
        }

        const auto site_opt = db->read_site(scheme, host);
        const auto site_id = site_opt ?
            *site_opt : co_await add_site(scheme, host);

        auto resource = uri.pathname();

        if (!uri.query().empty()) resource.append(
            fmt::format("?{}", uri.query())
        );
        if (!uri.fragment().empty()) resource.append(
            fmt::format("#{}", uri.fragment())
        );

        co_return db->create_source(site_id, resource);
    }

    auto api::add_site(
        std::string_view scheme,
        std::string_view host
    ) -> ext::task<std::string> {
        auto icon_id = std::optional<UUID::uuid>();
        auto bucket = co_await objects->connect();

        co_await dl->get_site_icon(
            fmt::format("{}://{}", scheme, host),
            [&](auto& stream) -> ext::task<> {
                const auto object = co_await bucket.add(
                    {},
                    co_await stream.size(),
                    [&stream] (auto&& part) -> ext::task<> {
                        co_await stream.read(
                            [&part](auto&& chunk) -> ext::task<> {
                                co_await part.write(chunk);
                            }
                        );
                    }
                );

                icon_id = object.id;
            }
        );

        co_return db->create_site(scheme, host, icon_id).id;
    }

    auto api::add_tag(std::string_view name) -> ext::task<UUID::uuid> {
        TIMBER_FUNC();

        const auto formatted = format_tag(name);
        const auto id = db->create_tag(formatted);

        co_await search->add_tag_alias(id, formatted);

        co_return id;
    }

    auto api::add_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        TIMBER_FUNC();

        const auto formatted = format_tag(alias);
        const auto name = db->create_tag_alias(tag_id, formatted);

        co_await search->add_tag_alias(tag_id, formatted);

        co_return name;
    }

    auto api::add_tag_source(
        const UUID::uuid& tag_id,
        std::string_view url
    ) -> ext::task<source> {
        TIMBER_FUNC();

        const auto src = co_await add_source(url);
        db->create_tag_source(tag_id, src.id);
        co_return src;
    }

    auto api::delete_post(const UUID::uuid& id) -> ext::task<> {
        TIMBER_FUNC();

        db->delete_post(id);
        co_await search->delete_post(id);
    }

    auto api::delete_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects
    ) -> ext::task<decltype(post::date_modified)> {
        TIMBER_FUNC();

        const auto modified = db->delete_post_objects(post_id, objects);
        co_await search->update_post_date_modified(post_id, modified);
        co_return modified;
    }

    auto api::delete_post_objects(
        const UUID::uuid& post_id,
        std::span<range> ranges
    ) -> ext::task<decltype(post::date_modified)> {
        TIMBER_FUNC();

        const auto modified = db->delete_post_objects_ranges(post_id, ranges);
        co_await search->update_post_date_modified(post_id, modified);
        co_return modified;
    }

    auto api::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        TIMBER_FUNC();

        db->delete_post_tag(post_id, tag_id);
        co_await search->remove_post_tag(post_id, tag_id);
    }

    auto api::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> void {
        TIMBER_FUNC();

        db->delete_related_post(post_id, related);
    }

    auto api::delete_tag(const UUID::uuid& id) -> ext::task<> {
        TIMBER_FUNC();

        db->delete_tag(id);
        co_await search->delete_tag(id);
    }

    auto api::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        TIMBER_FUNC();

        const auto name = db->delete_tag_alias(tag_id, alias);
        co_await search->delete_tag_alias(tag_id, alias);
        co_return name;
    }

    auto api::delete_tag_source(
        const UUID::uuid& tag_id,
        std::string_view source_id
    ) -> void {
        TIMBER_FUNC();

        db->delete_tag_source(tag_id, source_id);
    }

    auto api::get_bucket_id() const noexcept -> const UUID::uuid& {
        return objects->get_bucket_id();
    }

    auto api::get_comment(const UUID::uuid& comment_id) -> comment {
        TIMBER_FUNC();

        return db->read_comment(comment_id);
    }

    auto api::get_comments(const UUID::uuid& post_id) -> comment_tree {
        TIMBER_FUNC();

        const auto entities = db->read_comments(post_id);
        return build_tree(entities);
    }

    auto api::get_object(const UUID::uuid& object_id) -> ext::task<object> {
        TIMBER_FUNC();

        auto bucket = co_await objects->connect();

        const auto obj = db->read_object(object_id);
        const auto metadata = co_await bucket.meta(object_id);

        co_return object {
            .id = object_id,
            .hash = metadata.hash,
            .size = data_size(metadata.size),
            .type = metadata.type,
            .subtype = metadata.subtype,
            .date_added = metadata.date_added,
            .preview_id = obj.preview_id,
            .src = obj.src,
            .posts = co_await get_posts(
                bucket,
                db->read_object_posts(object_id)
            )
        };
    }

    auto api::get_object_preview_errors() -> std::vector<object_error> {
        TIMBER_FUNC();

        return db->read_object_preview_errors();
    }

    auto api::get_post(const UUID::uuid& id) -> ext::task<post> {
        TIMBER_FUNC();

        auto bucket = co_await objects->connect();
        auto data = db->read_post(id);
        auto posts = co_await get_posts(bucket, db->read_related_posts(id));

        auto result = post {
            .id = data.id,
            .title = data.title,
            .description = data.description,
            .date_created = data.date_created,
            .date_modified = data.date_modified,
            .posts = std::move(posts),
            .tags = db->read_post_tags(id)
        };

        auto objects = db->read_post_objects(id);

        for (auto&& obj : objects) {
            auto meta = co_await bucket.meta(obj.id);
            result.objects.push_back(object_preview {
                .id = std::move(obj.id),
                .preview_id = std::move(obj.preview_id),
                .type = std::move(meta.type),
                .subtype = std::move(meta.subtype)
            });
        }

        co_return result;
    }

    auto api::get_posts(
        bucket& bucket,
        std::vector<repo::db::post_preview>&& posts
    ) -> ext::task<std::vector<post_preview>> {
        auto result = std::vector<post_preview>();

        for (auto&& post : posts) {
            const auto obj = post.preview ?
                post.preview->id :
                std::optional<UUID::uuid>();

            auto preview = std::optional<object_preview>();

            if (obj) {
                auto metadata = co_await bucket.meta(*obj);

                preview = object_preview {
                    .id = std::move(metadata.id),
                    .preview_id = post.preview->preview_id,
                    .type = std::move(metadata.type),
                    .subtype = std::move(metadata.subtype)
                };
            }

            result.push_back(post_preview {
                .id = std::move(post.id),
                .title = std::move(post.title),
                .preview = std::move(preview),
                .comment_count = post.comment_count,
                .object_count = post.object_count,
                .date_created = post.date_created
            });
        }

        co_return result;
    }

    auto api::get_posts(
        const post_query& query
    ) -> ext::task<search_result<post_preview>> {
        TIMBER_FUNC();

        const auto result = co_await search->find_posts(query);
        auto bucket = co_await objects->connect();
        auto posts = co_await get_posts(bucket, db->read_posts(result.hits));

        co_return search_result<post_preview> {
            .total = result.total,
            .hits = std::move(posts)
        };
    }

    auto api::get_tag(const UUID::uuid& id) -> tag {
        TIMBER_FUNC();

        auto tag = db->read_tag(id);
        auto sources = db->read_tag_sources(id);

        auto result = minty::tag {
            .id = std::move(tag.id),
            .name = std::move(tag.name),
            .aliases = std::move(tag.aliases),
            .description = std::move(tag.description),
            .avatar = std::move(tag.avatar),
            .banner = std::move(tag.banner),
            .post_count = tag.post_count,
            .date_created = tag.date_created
        };

        for (const auto& src : sources) {
            result.sources.emplace_back(src);
        }

        return result;
    }

    auto api::get_tags(
        const tag_query& query
    ) -> ext::task<search_result<tag_preview>> {
        TIMBER_FUNC();

        const auto result = co_await search->find_tags(query);
        co_return search_result<tag_preview> {
            .total = result.total,
            .hits = db->read_tag_previews(result.hits)
        };
    }

    auto api::move_post_object(
        const UUID::uuid& post_id,
        unsigned int old_index,
        unsigned int new_index
    ) -> ext::task<> {
        TIMBER_FUNC();

        co_await search->update_post_date_modified(
            post_id,
            db->move_post_object(post_id, old_index, new_index)
        );
    }

    auto api::move_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        const std::optional<UUID::uuid>& destination
    ) -> ext::task<decltype(post::date_modified)> {
        TIMBER_FUNC();

        const auto date_modified = db->move_post_objects(
            post_id,
            objects,
            destination
        );

        co_await search->update_post_date_modified(post_id, date_modified);
        co_return date_modified;
    }

    auto api::prune() -> ext::task<> {
        TIMBER_FUNC();

        db->prune();

        auto bucket = co_await objects->connect();
        auto result = fstore::remove_result();

        const auto on_deleted = [&bucket, &result](
            auto objects
        ) -> ext::task<bool> {
            try {
                result = co_await bucket.remove(objects);
                co_return true;
            }
            catch (const std::runtime_error& ex) {
                TIMBER_ERROR("Object pruning has failed: {}", ex.what());
            }

            co_return false;
        };

        co_await db->prune_objects(on_deleted);

        if (result.objects_removed == 0) {
            TIMBER_INFO("No objects to prune");
            co_return;
        }

        TIMBER_INFO(
            "Removed {} {} freeing {}",
            result.objects_removed,
            result.objects_removed == 1 ? "object" : "objects",
            data_size(result.space_freed).formatted
        );
    }

    auto api::regenerate_preview(
        const UUID::uuid& object_id
    ) -> ext::task<decltype(object::preview_id)> {
        TIMBER_FUNC();

        auto bucket = co_await objects->connect();
        const auto metadata = co_await bucket.meta(object_id);

        try {
            const auto preview = co_await generate_preview(bucket, metadata);
            db->update_object_preview(object_id, preview);
            db->delete_object_preview_error(object_id);

            co_return preview;
        }
        catch (const std::exception& ex) {
            db->create_object_preview_error(object_id, ex.what());

            throw ex;
        }

        co_return decltype(object::preview_id){};
    }

    auto api::regenerate_preview(
        bucket& bucket,
        const repo::db::object_preview& object
    ) -> ext::task<bool> {
        TIMBER_FUNC();

        const auto reg = bucket.register_scoped();

        try {
            const auto metadata = co_await bucket.meta(object.id);
            const auto preview = co_await generate_preview(bucket, metadata);

            if (preview != object.preview_id) {
                db->update_object_preview(object.id, preview);
            }

            db->delete_object_preview_error(object.id);

            co_return true;
        }
        catch (const std::exception& ex) {
            db->create_object_preview_error(object.id, ex.what());
        }

        co_return false;
    }

    auto api::regenerate_preview_task(
        netcore::thread_pool& workers,
        repo::db::object_preview obj,
        std::size_t& errors,
        progress& progress,
        netcore::event<>& finished
    ) -> ext::detached_task {
        TIMBER_FUNC();

        auto bucket = co_await objects->connect();
        bucket.deregister();

        const auto success =
            co_await workers.wait(regenerate_preview(bucket, obj));

        if (!success) errors += 1;
        progress.completed += 1;

        if (progress.completed == progress.total) finished.emit();
    }

    auto api::regenerate_previews(
        int jobs,
        progress& progress
    ) -> ext::task<std::size_t> {
        TIMBER_FUNC();

        progress.total = db->read_total_objects();
        std::size_t errors = 0;

        auto workers = netcore::thread_pool(jobs, 32, "worker");
        auto finished = netcore::event();
        auto objects = db->read_objects(100);

        while (objects) {
            for (auto&& obj : objects()) {
                regenerate_preview_task(
                    workers,
                    obj,
                    errors,
                    progress,
                    finished
                );
            }

            co_await netcore::yield();
        }

        co_await finished.listen();

        co_return errors;
    }

    auto api::reindex() -> ext::task<> {
        TIMBER_FUNC();

        constexpr auto batch_size = 500;

        co_await search->delete_indices();
        co_await search->create_indices();

        {
            TIMBER_INFO("Reindexing tags...");
            auto tags = db->read_tag_text(batch_size);
            while (tags) {
                const auto errors = co_await search->add_tags(tags());

                for (const auto& error : errors) {
                    TIMBER_ERROR("tag {}", error);
                }
            }
        }

        {
            TIMBER_INFO("Reindexing posts...");
            auto posts = db->read_post_search(batch_size);
            while (posts) {
                const auto errors = co_await search->add_posts(posts());

                for (const auto& error : errors) {
                    TIMBER_ERROR("post {}", error);
                }
            }
        }
    }

    auto api::set_comment_content(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> std::string {
        TIMBER_FUNC();

        const auto formatted = format_comment(content);
        db->update_comment(comment_id, formatted);

        return formatted;
    }

    auto api::set_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> ext::task<modification<std::optional<std::string>>> {
        TIMBER_FUNC();

        const auto update = db->update_post_description(
            post_id,
            format_description(description)
        );
        co_await search->update_post_description(update);
        co_return modification<std::optional<std::string>> {
            .date_modified = update.date_modified,
            .new_value = update.new_data
        };
    }

    auto api::set_post_title(
        const UUID::uuid& post_id,
        std::string_view title
    ) -> ext::task<modification<std::optional<std::string>>> {
        TIMBER_FUNC();

        const auto update = db->update_post_title(post_id, format_title(title));
        co_await search->update_post_title(update);

        co_return modification<std::optional<std::string>> {
            .date_modified = update.date_modified,
            .new_value = update.new_data
        };
    }

    auto api::set_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> std::optional<std::string> {
        TIMBER_FUNC();

        return db->update_tag_description(
            tag_id,
            format_description(description)
        );
    }

    auto api::set_tag_name(
        const UUID::uuid& tag_id,
        std::string_view new_name
    ) -> ext::task<tag_name> {
        TIMBER_FUNC();

        const auto formatted = format_tag(new_name);
        const auto update  = db->update_tag_name(tag_id, formatted);

        if (update.old_name) {
            co_await search->update_tag_name(
                tag_id,
                update.old_name.value(),
                formatted
            );
        }

        co_return update.names;
    }
}
