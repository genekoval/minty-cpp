#pragma once

#include <minty/repo/db/model.h>

#include <pqxx/pqxx>

namespace minty::repo::db {
    class database {
        pqxx::connection connection;
        pqxx::nontransaction ntx;
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
        ) -> std::string;

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

        auto delete_tag(std::string_view tag_id) -> void;

        auto delete_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> tag_name;

        auto delete_tag_source(
            std::string_view tag_id,
            std::string_view source_id
        ) -> void;

        auto read_comments(std::string_view post_id) -> std::vector<comment>;

        auto read_object(std::string_view object_id) -> object;

        auto read_object_posts(
            std::string_view object_id
        ) -> std::vector<post_preview>;

        auto read_post(std::string_view post_id) -> post;

        auto read_post_objects(
            std::string_view post_id
        ) -> std::vector<object_preview>;

        auto read_post_tags(
            std::string_view post_id
        ) -> std::vector<tag_preview>;

        auto read_site(
            std::string_view scheme,
            std::string_view host
        ) -> std::optional<std::string>;

        auto read_tag(std::string_view tag_id) -> tag;

        auto read_tag_posts(
            std::string_view tag_id
        ) -> std::vector<post_preview>;

        auto read_tag_previews(
            const std::vector<std::string>& tags
        ) -> std::vector<tag_preview>;

        auto read_tag_sources(
            std::string_view tag_id
        ) -> std::vector<source>;

        auto read_tag_previews_all() -> std::vector<tag_preview>;

        auto update_tag_description(
            std::string_view tag_id,
            std::string_view description
        ) -> std::optional<std::string>;

        auto update_tag_name(
            std::string_view tag_id,
            std::string_view name
        ) -> tag_name;
    };
}
