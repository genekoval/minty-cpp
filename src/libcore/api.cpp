#include <minty/core/api.h>
#include <minty/core/downloader.h>
#include <minty/core/preview.h>
#include <minty/core/search.h>

#include <ext/string.h>

namespace minty::core {
    api::api(
        repo::db::database& db,
        fstore::bucket& bucket,
        downloader& dl,
        search_engine& search
    ) :
        db(&db),
        bucket(&bucket),
        dl(&dl),
        previews(bucket),
        search(&search)
    {}

    auto api::add_comment(
        std::string_view post_id,
        std::optional<std::string_view> parent_id,
        std::string_view content
    ) -> comment {
        const auto comment = db->create_comment(post_id, parent_id, content);
        return {
            comment.id,
            comment.content,
            comment.indent,
            comment.date_created
        };
    }

    auto api::add_object_data(
        std::size_t stream_size,
        std::function<void(fstore::part&&)> pipe
    ) -> std::string {
        const auto object = bucket->add({}, stream_size, pipe);
        db->create_object(object.id, previews.generate_preview(object), {});
        return object.id;
    }

    auto api::add_object_local(std::string_view path) -> std::string {
        const auto object = bucket->add(path);
        db->create_object(object.id, previews.generate_preview(object), {});
        return object.id;
    }

    auto api::add_object_url(std::string_view url) -> std::vector<std::string> {
        auto objects = std::vector<std::string>();

        dl->fetch(url, [&](auto& stream) {
            objects.push_back(
                add_object_data(stream.size(), [&stream](auto&& part) {
                    stream.read([&part](auto&& chunk) {
                        part.write(chunk);
                    });
                })
            );
        });

        return objects;
    }

    auto api::add_post(post_parts parts) -> std::string {
        return db->create_post(
            ext::trim(parts.title),
            ext::trim(parts.description),
            parts.objects,
            parts.tags
        );
    }

    auto api::add_tag(std::string_view name) -> std::string {
        const auto formatted_name = ext::trim(std::string(name));
        const auto id = db->create_tag(formatted_name);

        search->add_tag({
            .id = id,
            .names = {formatted_name}
        });

        return id;
    }

    auto api::delete_post(std::string_view id) -> void {
        db->delete_post(id);
    }

    auto api::delete_tag(std::string_view id) -> void {
        db->delete_tag(id);
        search->delete_tag(id);
    }

    auto api::get_comments(std::string_view post_id) -> comment_tree {
        const auto entities = db->read_comments(post_id);
        return build_tree(entities);
    }

    auto api::get_object_metadata(
        std::span<const repo::db::object> db_objects
    ) -> std::vector<object> {
        auto objects = std::vector<object>();

        for (const auto& obj : db_objects) {
            const auto meta = bucket->meta(obj.id);
            objects.emplace_back(
                meta.id,
                meta.hash,
                meta.size,
                meta.mime_type,
                meta.date_added,
                obj.preview_id,
                obj.src
            );
        }

        return objects;
    }

    auto api::get_post(std::string_view id) -> post {
        auto data = db->read_post(id);
        auto objects = db->read_objects(id);

        return post {
            .id = data.id,
            .title = data.title,
            .description = data.description,
            .date_created = data.date_created,
            .date_modified = data.date_modified,
            .objects = get_object_metadata(objects),
            .tags = data.tags
        };
    }

    auto api::get_tag(std::string_view id) -> tag {
        return db->read_tag(id);
    }

    auto api::get_tags_by_name(
        std::string_view term
    ) -> std::vector<tag_preview> {
        const auto ids = search->find_tags_by_name(term);
        return db->read_tag_previews(ids);
    }

    auto api::get_tag_posts(
        std::string_view tag_id
    ) -> std::vector<post_preview> {
        return db->read_tag_posts(tag_id);
    }

    auto api::get_tag_previews() -> std::vector<tag_preview> {
        return db->read_tag_previews_all();
    }
}
