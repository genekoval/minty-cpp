#pragma once

#include <minty/repo/db/data.h>

#include <pqxx/pqxx>

namespace minty::repo::db {
    class database {
        pqxx::connection connection;
    public:
        database(std::string_view connection_string);

        auto add_object(
            std::string_view object_id,
            std::optional<std::string_view> preview_id,
            std::optional<data::source> src
        ) -> object;

        auto create_site(
            std::string_view name,
            std::string_view homepage,
            std::optional<std::string_view> thumbnail_id
        ) -> site;

        auto create_tag(
            std::string_view name,
            std::string_view color
        ) -> tag;
    };
}
