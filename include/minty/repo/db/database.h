#pragma once

#include <minty/repo/db/model.h>
#include <minty/test.h>

#include <pqxx/pqxx>
#include <span>

namespace minty::repo::db {
    class database {
        std::optional<pqxx::connection> connection;

        auto ntx() -> pqxx::nontransaction;
    public:
        database() = default;

        database(std::string_view connection_string);

        database(const database&) = delete;

        database(database&& other) = default;

        VIRTUAL_DESTRUCTOR(database)

        VIRTUAL auto create_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) -> comment;

        VIRTUAL auto create_object(
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id,
            const std::optional<std::string_view>& source_id
        ) -> void;

        VIRTUAL auto create_post(
            std::string_view title,
            std::string_view description,
            const std::vector<UUID::uuid>& objects,
            const std::vector<UUID::uuid>& tags
        ) -> post_search;

        VIRTUAL auto create_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            std::int16_t position
        ) -> std::string;

        VIRTUAL auto create_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> void;

        VIRTUAL auto create_reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) -> comment;

        VIRTUAL auto create_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        VIRTUAL auto create_site(
            std::string_view scheme,
            std::string_view name,
            std::optional<UUID::uuid> icon
        ) -> site;

        VIRTUAL auto create_source(
            std::string_view site_id,
            std::string_view resource
        ) -> source;

        VIRTUAL auto create_tag(std::string_view name) -> UUID::uuid;

        VIRTUAL auto create_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> tag_name;

        VIRTUAL auto create_tag_source(
            const UUID::uuid& tag_id,
            std::string_view source_id
        ) -> void;

        VIRTUAL auto delete_post(const UUID::uuid& post_id) -> void;

        VIRTUAL auto delete_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects
        ) -> std::string;

        VIRTUAL auto delete_post_objects_ranges(
            const UUID::uuid& post_id,
            std::span<const range> ranges
        ) -> std::string;

        VIRTUAL auto delete_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> void;

        VIRTUAL auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        VIRTUAL auto delete_tag(const UUID::uuid& tag_id) -> void;

        VIRTUAL auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> tag_name;

        VIRTUAL auto delete_tag_source(
            const UUID::uuid& tag_id,
            std::string_view source_id
        ) -> void;

        VIRTUAL auto move_post_object(
            const UUID::uuid& post_id,
            unsigned int old_index,
            unsigned int new_index
        ) -> std::string;

        VIRTUAL auto move_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            const std::optional<UUID::uuid>& destination
        ) -> std::string;

        VIRTUAL auto prune() -> void;

        VIRTUAL auto prune_objects(
            std::function<bool(std::span<const UUID::uuid>)>&& on_deleted
        ) -> void;

        VIRTUAL auto read_comments(
            const UUID::uuid& post_id
        ) -> std::vector<comment>;

        VIRTUAL auto read_object(const UUID::uuid& object_id) -> object;

        VIRTUAL auto read_object_posts(
            const UUID::uuid& object_id
        ) -> std::vector<post_preview>;

        VIRTUAL auto read_post(const UUID::uuid& post_id) -> post;

        VIRTUAL auto read_posts(
            const std::vector<UUID::uuid>& posts
        ) -> std::vector<post_preview>;

        VIRTUAL auto read_post_objects(
            const UUID::uuid& post_id
        ) -> std::vector<object_preview>;

        VIRTUAL auto read_post_search() -> std::vector<post_search>;

        VIRTUAL auto read_post_tags(
            const UUID::uuid& post_id
        ) -> std::vector<tag_preview>;

        VIRTUAL auto read_related_posts(
            const UUID::uuid& post_id
        ) -> std::vector<post_preview>;

        VIRTUAL auto read_site(
            std::string_view scheme,
            std::string_view host
        ) -> std::optional<std::string>;

        VIRTUAL auto read_tag(const UUID::uuid& tag_id) -> tag;

        VIRTUAL auto read_tag_previews(
            const std::vector<UUID::uuid>& tags
        ) -> std::vector<tag_preview>;

        VIRTUAL auto read_tag_sources(
            const UUID::uuid& tag_id
        ) -> std::vector<source>;

        VIRTUAL auto read_tag_text() -> std::vector<tag_text>;

        VIRTUAL auto update_comment(
            const UUID::uuid& comment_id,
            std::string_view content
        ) -> void;

        VIRTUAL auto update_object_preview(
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id
        ) -> void;

        VIRTUAL auto update_post_description(
            const UUID::uuid& post_id,
            std::string_view description
        ) -> post_update;

        VIRTUAL auto update_post_title(
            const UUID::uuid& post_id,
            std::string_view title
        ) -> post_update;

        VIRTUAL auto update_tag_description(
            const UUID::uuid& tag_id,
            std::string_view description
        ) -> std::optional<std::string>;

        VIRTUAL auto update_tag_name(
            const UUID::uuid& tag_id,
            std::string_view name
        ) -> tag_name_update;
    };
}
