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
            std::string_view post_id,
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

        auto add_post(post_parts parts) -> std::string;

        auto add_post_objects(
            std::string_view post_id,
            const std::vector<std::string>& objects,
            std::int16_t position
        ) -> std::string;

        auto add_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        auto add_related_post(
            std::string_view post_id,
            std::string_view related
        ) -> void;

        auto add_reply(
            std::string_view parent_id,
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

        auto delete_post(std::string_view id) -> void;

        auto delete_post_objects(
            std::string_view post_id,
            const std::vector<std::string>& objects
        ) -> std::string;

        auto delete_post_objects(
            std::string_view post_id,
            std::span<range> ranges
        ) -> std::string;

        auto delete_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        auto delete_related_post(
            std::string_view post_id,
            std::string_view related
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

        auto get_comments(std::string_view post_id) -> comment_tree;

        auto get_object(std::string_view object_id) -> object;

        auto get_post(std::string_view id) -> post;

        auto get_posts(const post_query& query) -> search_result<post_preview>;

        auto get_tag(std::string_view id) -> tag;

        auto get_tags(const tag_query& query) -> search_result<tag_preview>;

        auto move_post_object(
            std::string_view post_id,
            unsigned int old_index,
            unsigned int new_index
        ) -> void;

        auto move_post_objects(
            std::string_view post_id,
            const std::vector<std::string>& objects,
            std::optional<std::string> destination
        ) -> std::string;

        auto prune() -> void;

        auto regenerate_preview(
            std::string_view object_id
        ) -> std::optional<std::string>;

        auto reindex() -> void;

        auto set_comment_content(
            std::string_view comment_id,
            std::string_view content
        ) -> std::string;

        auto set_post_description(
            std::string_view post_id,
            std::string_view description
        ) -> modification<std::optional<std::string>>;

        auto set_post_title(
            std::string_view post_id,
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
