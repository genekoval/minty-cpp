#pragma once

#include <minty/repo/db/model.h>

#include <pqxx/pqxx>
#include <span>

namespace minty::repo::db {
    class database {
        pqxx::connection connection;

        auto ntx() -> pqxx::nontransaction;

        auto read_post_date_modified(
            pqxx::transaction_base& tx,
            std::string_view post_id
        ) -> std::string;
    public:
        database(std::string_view connection_string);

        database(const database&) = delete;

        database(database&& other) = default;

        auto create_comment(
            std::string_view post_id,
            std::optional<std::string_view> parent_id,
            std::string_view content
        ) -> comment;

        auto create_object(
            std::string_view object_id,
            std::optional<std::string_view> preview_id,
            std::optional<std::string_view> source_id
        ) -> void;

        auto create_post(
            std::optional<std::string_view> title,
            std::optional<std::string_view> description,
            const std::vector<std::string>& objects,
            const std::vector<std::string>& tags
        ) -> post_search;

        auto create_post_objects(
            std::string_view post_id,
            const std::vector<std::string>& objects,
            unsigned int position
        ) -> post_object_update;

        auto create_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        auto create_site(
            std::string_view scheme,
            std::string_view name,
            std::optional<std::string_view> icon
        ) -> site;

        auto create_source(
            std::string_view site_id,
            std::string_view resource
        ) -> source;

        auto create_tag(std::string_view name) -> std::string;

        auto create_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> tag_name;

        auto create_tag_source(
            std::string_view tag_id,
            std::string_view source_id
        ) -> void;

        auto delete_post(std::string_view post_id) -> void;

        auto delete_post_objects(
            std::string_view post_id,
            std::span<const range> ranges
        ) -> std::string;

        auto delete_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        auto delete_tag(std::string_view tag_id) -> void;

        auto delete_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> tag_name;

        auto delete_tag_source(
            std::string_view tag_id,
            std::string_view source_id
        ) -> void;

        auto move_post_object(
            std::string_view post_id,
            unsigned int old_index,
            unsigned int new_index
        ) -> std::string;

        auto prune() -> void;

        auto prune_objects(
            std::function<bool(std::span<const std::string>)>&& on_deleted
        ) -> void;

        auto read_comments(std::string_view post_id) -> std::vector<comment>;

        auto read_object(std::string_view object_id) -> object;

        auto read_object_posts(
            std::string_view object_id
        ) -> std::vector<post_preview>;

        auto read_post(std::string_view post_id) -> post;

        auto read_posts(
            const std::vector<std::string>& posts
        ) -> std::vector<post_preview>;

        auto read_post_objects(
            std::string_view post_id
        ) -> std::vector<object_preview>;

        auto read_post_search() -> std::vector<post_search>;

        auto read_post_tags(
            std::string_view post_id
        ) -> std::vector<tag_preview>;

        auto read_site(
            std::string_view scheme,
            std::string_view host
        ) -> std::optional<std::string>;

        auto read_tag(std::string_view tag_id) -> tag;

        auto read_tag_previews(
            const std::vector<std::string>& tags
        ) -> std::vector<tag_preview>;

        auto read_tag_sources(
            std::string_view tag_id
        ) -> std::vector<source>;

        auto read_tag_text() -> std::vector<tag_text>;

        auto update_comment(
            std::string_view comment_id,
            std::string_view content
        ) -> void;

        auto update_post_description(
            std::string_view post_id,
            std::string_view description
        ) -> post_update;

        auto update_post_title(
            std::string_view post_id,
            std::string_view title
        ) -> post_update;

        auto update_tag_description(
            std::string_view tag_id,
            std::string_view description
        ) -> std::optional<std::string>;

        auto update_tag_name(
            std::string_view tag_id,
            std::string_view name
        ) -> tag_name_update;
    };
}
