#pragma once

#include <minty/repo/db/model.h>

#include <pqxx/pqxx>

namespace minty::repo::db {
    class database {
        pqxx::connection connection;
    public:
        database(std::string_view connection_string);

        auto add_object(std::string_view object_id) -> void;

        auto create_site(
            std::string_view name,
            std::string_view homepage,
            std::optional<std::string_view> thumbnail_id
        ) -> site;

        auto create_tag(
            std::string_view name,
            std::string_view color
        ) -> tag;

        auto read_object(
            std::string_view object_id
        ) -> object;

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
