#pragma once

#include <minty/model.h>

#include <pqxx/pqxx>

namespace minty::repo::db {
    class database {
        pqxx::connection connection;
    public:
        database(std::string_view connection_string);

        auto create_site(
            std::string_view name,
            std::string_view homepage,
            std::optional<std::string_view> thumbnail_id
        ) -> model::site;

        auto create_tag(
            std::string_view name,
            std::string_view color
        ) -> model::tag;
    };
}
