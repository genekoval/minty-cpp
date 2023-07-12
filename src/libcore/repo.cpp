#include "utility.hpp"

#include <internal/core/comment_tree.hpp>
#include <internal/core/preview.hpp>
#include <internal/core/repo.hpp>

#include <minty/except.hpp>

#include <uri/uri>

namespace minty::core {
    repo::repo(
        db::database& database,
        object_store& objects,
        downloader& dl,
        search_engine& search
    ) :
        database(&database),
        objects(&objects),
        dl(&dl),
        search(&search)
    {}

    auto repo::add_comment(
        const UUID::uuid& post_id,
        std::string_view content
    ) -> ext::task<comment_data> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        const auto comment = co_await db.create_comment(
            post_id,
            format_comment(content)
        );

        co_return comment_data {
            comment.id,
            comment.content,
            comment.indent,
            comment.date_created
        };
    }

    auto repo::add_object(
        db::connection_type& db,
        bucket& bucket,
        fstore::object&& object,
        const std::optional<std::int64_t>& src
    ) -> ext::task<object_preview> {
        auto preview_id = std::optional<UUID::uuid>();
        auto error = std::optional<std::string>();

        try {
            preview_id = co_await generate_preview(bucket, object);
        }
        catch (const std::exception& ex) {
            error = ex.what();
        }

        co_await db.create_object(object.id, preview_id, src);
        if (error) co_await db.create_object_preview_error(object.id, *error);

        co_return object_preview {
            std::move(object.id),
            std::move(preview_id),
            std::move(object.type),
            std::move(object.subtype)
        };
    }

    auto repo::add_objects_url(
        std::string_view url
    ) -> ext::task<std::vector<object_preview>> {
        TIMBER_FUNC();

        co_return std::vector<object_preview> { co_await download_file(url) };
    }

    auto repo::add_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        const std::optional<UUID::uuid>& destination
    ) -> ext::task<decltype(post::date_modified)> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto date_modified =
            co_await db.create_post_objects(post_id, objects, destination);

        co_await search->update_post_date_modified(post_id, date_modified);

        co_await tx.commit();
        co_return date_modified;
    }

    auto repo::add_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        co_await db.create_post_tag(post_id, tag_id);
        co_await search->add_post_tag(post_id, tag_id);

        co_await tx.commit();
    }

    auto repo::add_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        co_await db.create_related_post(post_id, related);
    }

    auto repo::add_reply(
        const UUID::uuid& parent_id,
        std::string_view content
    ) -> ext::task<comment_data> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        const auto comment = co_await db.create_reply(
            parent_id,
            format_comment(content)
        );

        co_return comment_data {
            comment.id,
            comment.content,
            comment.indent,
            comment.date_created
        };
    }

    auto repo::add_source(
        db::connection_type& db,
        std::string_view url
    ) -> ext::task<source> {
        constexpr auto host_prefix = std::string_view("www.");

        const auto uri = uri::uri(std::string(url));

        const auto scheme = uri.scheme();
        auto host = uri.host();
        if (host.starts_with(host_prefix)) {
            host = host.substr(host_prefix.size());
        }

        const auto site_opt = co_await db.read_site(scheme, host);
        const auto site_id = site_opt ?
            *site_opt : co_await add_site(db, scheme, host);

        auto resource = uri.pathname();

        if (!uri.query().empty()) resource.append(
            fmt::format("?{}", uri.query())
        );
        if (!uri.fragment().empty()) resource.append(
            fmt::format("#{}", uri.fragment())
        );

        co_return co_await db.create_source(site_id, resource);
    }

    auto repo::add_site(
        db::connection_type& db,
        std::string_view scheme,
        std::string_view host
    ) -> ext::task<std::int64_t> {
        auto bucket = co_await objects->connect();

        const auto icon = co_await dl->get_site_icon(scheme, host, bucket);
        const auto site = co_await db.create_site(scheme, host, icon);

        co_return site.id;
    }

    auto repo::add_tag(std::string_view name) -> ext::task<UUID::uuid> {
        TIMBER_FUNC();

        const auto formatted = format_tag(name);

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto id = co_await db.create_tag(formatted);
        co_await search->add_tag_alias(id, formatted);

        co_await tx.commit();
        co_return id;
    }

    auto repo::add_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        TIMBER_FUNC();

        const auto formatted = format_tag(alias);

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto name = co_await db.create_tag_alias(tag_id, formatted);
        co_await search->add_tag_alias(tag_id, formatted);

        co_await tx.commit();
        co_return name;
    }

    auto repo::add_tag_source(
        const UUID::uuid& tag_id,
        std::string_view url
    ) -> ext::task<source> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        const auto src = co_await add_source(db, url);
        co_await db.create_tag_source(tag_id, src.id);
        co_return src;
    }

    auto repo::create_indices() -> ext::task<> {
        co_await search->delete_indices();
        co_await search->create_indices();
    }

    auto repo::create_post(const UUID::uuid& post_id) -> ext::task<> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto timestamp = co_await db.create_post(post_id);
        co_await search->publish_post(post_id, timestamp);

        co_await tx.commit();
    }

    auto repo::create_post_draft() -> ext::task<UUID::uuid> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto post = co_await db.create_post_draft();
        co_await search->add_post(post);

        co_await tx.commit();
        co_return post.id;
    }

    auto repo::delete_post(const UUID::uuid& id) -> ext::task<> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        co_await db.delete_post(id);
        co_await search->delete_post(id);

        co_await tx.commit();
    }

    auto repo::delete_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects
    ) -> ext::task<decltype(post::date_modified)> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto modified = co_await db.delete_post_objects(post_id, objects);
        co_await search->update_post_date_modified(post_id, modified);

        co_await tx.commit();
        co_return modified;
    }

    auto repo::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        co_await db.delete_post_tag(post_id, tag_id);
        co_await search->remove_post_tag(post_id, tag_id);

        co_await tx.commit();
    }

    auto repo::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        co_await db.delete_related_post(post_id, related);
    }

    auto repo::delete_tag(const UUID::uuid& id) -> ext::task<> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        co_await db.delete_tag(id);
        co_await search->delete_tag(id);

        co_await tx.commit();
    }

    auto repo::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto name = co_await db.delete_tag_alias(tag_id, alias);
        co_await search->delete_tag_alias(tag_id, alias);

        co_await tx.commit();
        co_return name;
    }

    auto repo::delete_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        co_await db.delete_tag_source(tag_id, source_id);
    }

    auto repo::download_file(
        std::string_view url
    ) -> ext::task<object_preview> {
        TIMBER_FUNC();

        auto bucket = co_await objects->connect();
        auto [status, metadata] = co_await dl->fetch(url, bucket);

        auto db = co_await database->connect();
        auto object = co_await add_object(
            db,
            bucket,
            std::move(metadata),
            std::nullopt
        );

        if (status.ok()) co_return object;
        throw download_error(url, status, object);
    }

    auto repo::get_bucket_id() const noexcept -> const UUID::uuid& {
        return objects->get_bucket_id();
    }

    auto repo::get_comment(const UUID::uuid& comment_id) -> ext::task<comment> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        co_return co_await db.read_comment(comment_id);
    }

    auto repo::get_comments(
        const UUID::uuid& post_id
    ) -> ext::task<comment_tree> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        const auto entities = co_await db.read_comments(post_id);
        co_return build_tree(entities);
    }

    auto repo::get_object(const UUID::uuid& object_id) -> ext::task<object> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto bucket = co_await objects->connect();

        const auto obj = co_await db.read_object(object_id);
        const auto metadata = co_await bucket.meta(object_id);

        auto result = object {
            .id = object_id,
            .hash = metadata.hash,
            .size = data_size(metadata.size),
            .type = metadata.type,
            .subtype = metadata.subtype,
            .date_added = metadata.date_added,
            .preview_id = obj.preview_id,
            .src = obj.src,
        };

        result.posts = co_await get_posts(
            bucket,
            co_await db.read_object_posts(object_id)
        );

        co_return result;
    }

    auto repo::get_object_preview_errors() ->
        ext::task<std::vector<object_error>>
    {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        co_return co_await db.read_object_preview_errors();
    }

    auto repo::get_post(const UUID::uuid& id) -> ext::task<post> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto bucket = co_await objects->connect();

        auto data = co_await db.read_post(id);
        auto posts = co_await get_posts(
            bucket,
            co_await db.read_related_posts(id)
        );

        auto result = post {
            .id = data.id,
            .title = data.title,
            .description = data.description,
            .visibility = data.visibility,
            .date_created = data.date_created,
            .date_modified = data.date_modified,
            .posts = std::move(posts)
        };

        result.tags = co_await db.read_post_tags(id);
        auto objects = co_await db.read_post_objects(id);

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

    auto repo::get_posts(
        bucket& bucket,
        std::vector<db::post_preview>&& posts
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
                .comment_count = static_cast<std::uint32_t>(post.comment_count),
                .object_count = static_cast<std::uint32_t>(post.object_count),
                .date_created = post.date_created
            });
        }

        co_return result;
    }

    auto repo::get_posts(
        const post_query& query
    ) -> ext::task<search_result<post_preview>> {
        TIMBER_FUNC();

        const auto result = co_await search->find_posts(query);

        auto db = co_await database->connect();
        auto bucket = co_await objects->connect();

        auto posts = co_await get_posts(
            bucket,
            co_await db.read_posts(result.hits)
        );

        co_return search_result<post_preview> {
            .total = result.total,
            .hits = std::move(posts)
        };
    }

    auto repo::get_tag(const UUID::uuid& id) -> ext::task<tag> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        auto tag = co_await db.read_tag(id);
        auto sources = co_await db.read_tag_sources(id);

        auto result = minty::tag {
            .id = std::move(tag.id),
            .name = std::move(tag.name),
            .aliases = std::move(tag.aliases),
            .description = std::move(tag.description),
            .avatar = std::move(tag.avatar),
            .banner = std::move(tag.banner),
            .post_count = static_cast<std::uint32_t>(tag.post_count),
            .date_created = tag.date_created
        };

        for (const auto& src : sources) {
            result.sources.emplace_back(src);
        }

        co_return result;
    }

    auto repo::get_tags(
        const tag_query& query
    ) -> ext::task<search_result<tag_preview>> {
        TIMBER_FUNC();

        const auto res = co_await search->find_tags(query);

        auto db = co_await database->connect();

        auto result = search_result<tag_preview> { .total = res.total };
        result.hits = co_await db.read_tag_previews(res.hits);

        co_return result;
    }

    auto repo::move_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        const std::optional<UUID::uuid>& destination
    ) -> ext::task<time_point> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto date_modified = co_await db.move_post_objects(
            post_id,
            objects,
            destination
        );
        co_await search->update_post_date_modified(post_id, date_modified);

        co_await tx.commit();
        co_return date_modified;
    }

    auto repo::prune() -> ext::task<> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        co_await db.prune();

        auto bucket = co_await objects->connect();
        auto tx = co_await db.begin();

        const auto objects = co_await db.prune_objects();
        const auto result = co_await bucket.remove(objects);

        co_await tx.commit();

        if (result.objects_removed == 0) {
            TIMBER_DEBUG("No objects to prune");
            co_return;
        }

        TIMBER_DEBUG(
            "Removed {} {} freeing {}",
            result.objects_removed,
            result.objects_removed == 1 ? "object" : "objects",
            data_size(result.space_freed).formatted
        );
    }

    auto repo::regenerate_preview(
        const UUID::uuid& object_id
    ) -> ext::task<std::optional<UUID::uuid>> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto bucket = co_await objects->connect();

        const auto metadata = co_await bucket.meta(object_id);

        auto exception = std::exception_ptr();
        auto exception_task = std::optional<ext::task<>>();

        try {
            const auto preview = co_await generate_preview(bucket, metadata);

            co_await db.update_object_preview(object_id, preview);
            co_await db.delete_object_preview_error(object_id);

            co_return preview;
        }
        catch (const std::exception& ex) {
            exception = std::current_exception();
            exception_task = db.create_object_preview_error(
                object_id,
                ex.what()
            );
        }

        co_await exception_task.value();
        std::rethrow_exception(exception);
    }

    auto repo::regenerate_preview(
        bucket& bucket,
        const db::object_preview& object
    ) -> ext::task<std::optional<UUID::uuid>> {
        TIMBER_FUNC();

        const auto metadata = co_await bucket.meta(object.id);
        co_return co_await generate_preview(bucket, metadata);
    }

    auto repo::regenerate_preview_task(
        netcore::async_thread_pool& workers,
        db::object_preview object,
        std::size_t& errors,
        progress& progress,
        unsigned int& counter,
        netcore::event<>& finished
    ) -> ext::detached_task {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto bucket = co_await objects->connect();

        auto preview = std::optional<UUID::uuid>();
        auto error_message = std::string();
        auto exception_task = std::optional<ext::task<>>();

        try {
            preview = co_await workers.wait(
                regenerate_preview(bucket, object)
            );
        }
        catch (const std::exception& ex) {
            error_message = ex.what();
            exception_task = db.create_object_preview_error(
                object.id,
                error_message
            );
        }

        if (exception_task) {
            co_await *exception_task;
            ++errors;
        }
        else {
            if (preview != object.preview_id) {
                co_await db.update_object_preview(object.id, preview);
            }

            co_await db.delete_object_preview_error(object.id);
        }

        ++progress.completed;
        --counter;
        if (counter == 0) finished.emit();
    }

    auto repo::regenerate_previews(
        unsigned int batch_size,
        unsigned int jobs,
        progress& progress
    ) -> ext::task<std::size_t> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        progress.total = co_await db.read_total_objects();
        if (progress.total == 0) co_return 0;

        std::size_t errors = 0;
        auto workers = netcore::async_thread_pool(
            std::min(static_cast<std::size_t>(jobs), progress.total),
            1024,
            "worker"
        );
        auto finished = netcore::event();
        auto portal = co_await db.read_objects(batch_size);

        while (portal) {
            const auto objects = co_await portal.next();
            unsigned int counter = objects.size();

            for (const auto& obj : objects) {
                regenerate_preview_task(
                    workers,
                    obj,
                    errors,
                    progress,
                    counter,
                    finished
                );
            }

            if (counter > 0) co_await finished.listen();
        }

        co_return errors;
    }

    auto repo::set_comment_content(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> ext::task<std::string> {
        TIMBER_FUNC();

        const auto formatted = format_comment(content);

        auto db = co_await database->connect();
        co_await db.update_comment(comment_id, formatted);

        co_return formatted;
    }

    auto repo::set_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> ext::task<modification<std::optional<std::string>>> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto update = co_await db.update_post_description(
            post_id,
            format_description(description)
        );
        co_await search->update_post_description(update);

        co_await tx.commit();
        co_return modification<std::optional<std::string>> {
            .date_modified = update.date_modified,
            .new_value = update.new_data
        };
    }

    auto repo::set_post_title(
        const UUID::uuid& post_id,
        std::string_view title
    ) -> ext::task<modification<std::optional<std::string>>> {
        TIMBER_FUNC();

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto update = co_await db.update_post_title(
            post_id,
            format_title(title)
        );
        co_await search->update_post_title(update);

        co_await tx.commit();
        co_return modification<std::optional<std::string>> {
            .date_modified = update.date_modified,
            .new_value = update.new_data
        };
    }

    auto repo::set_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> ext::task<std::optional<std::string>> {
        TIMBER_FUNC();

        auto db = co_await database->connect();

        co_return co_await db.update_tag_description(
            tag_id,
            format_description(description)
        );
    }

    auto repo::set_tag_name(
        const UUID::uuid& tag_id,
        std::string_view new_name
    ) -> ext::task<tag_name> {
        TIMBER_FUNC();

        const auto formatted = format_tag(new_name);

        auto db = co_await database->connect();
        auto tx = co_await db.begin();

        const auto update  = co_await db.update_tag_name(tag_id, formatted);

        if (update.old_name) {
            co_await search->update_tag_name(
                tag_id,
                *update.old_name,
                formatted
            );
        }

        co_await tx.commit();
        co_return update.names;
    }
}
