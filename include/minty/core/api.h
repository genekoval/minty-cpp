#pragma once

#include <minty/core/comment_tree.h>
#include <minty/core/downloader.h>
#include <minty/core/object_store.h>
#include <minty/core/model.h>
#include <minty/core/preview.h>
#include <minty/core/search.h>
#include <minty/repo/db/database.h>

namespace minty::core {
    struct source_parts {
        std::string site_id;
        std::string resource;
    };

    class api {
        repo::db::database* db;
        object_store* objects;
        downloader* dl;
        preview_service previews;
        search_engine* search;

        auto add_object(
            fstore::object_meta&& object,
            const std::optional<std::string>& src
        ) -> object_preview;

        auto add_site(
            std::string_view scheme,
            std::string_view host
        ) -> std::string;

        auto add_source(std::string_view url) -> source;

        auto get_posts(
            std::vector<repo::db::post_preview>&& posts
        ) -> std::vector<post_preview>;
    public:
        api(
            repo::db::database& db,
            object_store& objects,
            downloader& dl,
            search_engine& search
        );

        auto add_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) -> comment;

        auto add_object_data(
            std::size_t stream_size,
            std::function<void(fstore::part&&)> pipe
        ) -> object_preview;

        auto add_object_local(std::string_view path) -> object_preview;

        auto add_objects_url(
            std::string_view url
        ) -> std::vector<object_preview>;

        auto add_post(post_parts parts) -> UUID::uuid;

        auto add_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            std::int16_t position
        ) -> std::string;

        auto add_post_tag(
            const UUID::uuid& post_id,
            std::string_view tag_id
        ) -> void;

        auto add_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        auto add_reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) -> comment;

        auto add_tag(std::string_view name) -> std::string;

        auto add_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> tag_name;

        auto add_tag_source(
            std::string_view tag_id,
            std::string_view url
        ) -> source;

        auto delete_post(const UUID::uuid& id) -> void;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects
        ) -> std::string;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            std::span<range> ranges
        ) -> std::string;

        auto delete_post_tag(
            const UUID::uuid& post_id,
            std::string_view tag_id
        ) -> void;

        auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        auto delete_tag(std::string_view id) -> void;

        auto delete_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> tag_name;

        auto delete_tag_source(
            std::string_view tag_id,
            std::string_view source_id
        ) -> void;

        auto get_comments(const UUID::uuid& post_id) -> comment_tree;

        auto get_object(const UUID::uuid& object_id) -> object;

        auto get_post(const UUID::uuid& id) -> post;

        auto get_posts(const post_query& query) -> search_result<post_preview>;

        auto get_tag(std::string_view id) -> tag;

        auto get_tags(const tag_query& query) -> search_result<tag_preview>;

        auto move_post_object(
            const UUID::uuid& post_id,
            unsigned int old_index,
            unsigned int new_index
        ) -> void;

        auto move_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            const std::optional<UUID::uuid>& destination
        ) -> std::string;

        auto prune() -> void;

        auto regenerate_preview(
            const UUID::uuid& object_id
        ) -> std::optional<UUID::uuid>;

        auto reindex() -> void;

        auto set_comment_content(
            const UUID::uuid& comment_id,
            std::string_view content
        ) -> std::string;

        auto set_post_description(
            const UUID::uuid& post_id,
            std::string_view description
        ) -> modification<std::optional<std::string>>;

        auto set_post_title(
            const UUID::uuid& post_id,
            std::string_view title
        ) -> modification<std::optional<std::string>>;

        auto set_tag_description(
            std::string_view tag_id,
            std::string_view description
        ) -> std::optional<std::string>;

        auto set_tag_name(
            std::string_view tag_id,
            std::string_view new_name
        ) -> tag_name;
    };
}
