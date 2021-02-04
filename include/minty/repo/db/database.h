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

        auto add_object(std::string_view object_id) -> void;

        auto create_comment(
            std::string_view post_id,
            std::optional<std::string_view> parent_id,
            std::string_view content
        ) -> comment;

        auto create_creator(std::string_view name) -> std::string;

        auto create_creator_aliases(
            std::string_view creator_id,
            const std::vector<std::string>& aliases
        ) -> void;

        auto create_creator_source(
            std::string_view creator_id,
            std::string_view site_id,
            std::string_view url
        ) -> void;

        auto create_post(
            std::optional<std::string_view> description,
            const std::vector<std::string>& objects,
            std::optional<std::string_view> creator_id,
            const std::vector<std::string>& tags
        ) -> std::string;

        auto create_site(
            std::string_view name,
            std::string_view homepage,
            std::optional<std::string_view> thumbnail_id
        ) -> site;

        auto create_tag(
            std::string_view name,
            std::string_view color
        ) -> tag;

        auto read_creator(std::string_view creator_id) -> creator;

        auto read_creator_posts(
            std::string_view creator_id
        ) -> std::vector<post_preview>;

        auto read_creator_previews_all() -> std::vector<creator_preview>;

        auto read_object(std::string_view object_id) -> object;

        auto read_post(std::string_view post_id) -> post;

        auto update_object_preview(
            std::string_view object_id,
            std::string_view preview_id
        ) -> void;

        auto update_object_source(
            std::string_view object_id,
            const site& website,
            std::string_view url
        ) -> void;
    };
}
