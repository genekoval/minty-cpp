#include <minty/core/api.h>

#include <ext/string.h>

namespace minty::core {
    api::api(repo::db::database& db, fstore::bucket& bucket) :
        db(&db),
        bucket(&bucket)
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

    auto api::add_creator(std::string_view name) -> std::string {
        return db->create_creator(ext::trim(std::string(name)));
    }

    auto api::add_object_data(
        std::size_t stream_size,
        std::function<void(fstore::part&&)> pipe
    ) -> std::string {
        const auto object = bucket->add({}, stream_size, pipe);
        db->create_object(object.id, {}, {});
        return object.id;
    }

    auto api::add_object_local(std::string_view path) -> std::string {
        const auto object = bucket->add(path);
        db->create_object(object.id, {}, {});
        return object.id;
    }

    auto api::add_object_url(std::string_view url) -> std::string {
        // TODO: Use an external service to handle URLs.
        throw std::runtime_error("Not implemented.");
    }

    auto api::add_post(post_parts parts) -> std::string {
        if (parts.description) {
            auto formatted = ext::trim(parts.description.value());
            // An empty string is equivalent to no value.
            if (formatted.empty()) parts.description.reset();
            else parts.description = formatted;
        }

        return db->create_post(
            parts.description,
            parts.objects,
            parts.creators,
            parts.tags
        );
    }

    auto api::add_tag(std::string_view name, std::string_view color) -> tag {
        return db->create_tag(name, color);
    }

    auto api::get_comments(std::string_view post_id) -> comment_tree {
        const auto entities = db->read_comments(post_id);
        return build_tree(entities);
    }

    auto api::get_creator(std::string_view id) -> creator {
        return db->read_creator(id);
    }

    auto api::get_creator_posts(
        std::string_view creator_id
    ) -> std::vector<post_preview> {
        return db->read_creator_posts(creator_id);
    }

    auto api::get_creator_previews() -> std::vector<creator_preview> {
        return db->read_creator_previews_all();
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
            .description = data.description,
            .date_created = data.date_created,
            .date_modified = data.date_modified,
            .objects = get_object_metadata(objects),
            .tags = data.tags,
            .creators = data.creators
        };
    }
}
