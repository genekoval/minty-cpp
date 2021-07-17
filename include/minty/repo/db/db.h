#pragma once

#include <minty/error.h>

#include <pqxx/pqxx>

namespace minty::repo::db {
    class connection_initializer {
        pqxx::connection* connection;
    public:
        connection_initializer(pqxx::connection& c);

        auto prepare(const std::string& name, unsigned int argc) -> void;
    };

    struct sql_error {
        const std::string context;
        const std::string detail;
        const std::string error;
    };

    auto parse_error(const pqxx::sql_error& error) -> sql_error;

    class unique_entity_violation : public minty::unique_entity_violation {
        static auto parse_error (
            const sql_error& error
        ) -> std::array<std::string, 2>;

        unique_entity_violation(
            std::string_view entity,
            const std::array<std::string, 2> kv
        );

        unique_entity_violation(
            std::string_view entity_name,
            const sql_error& error
        );
    public:
        unique_entity_violation(
            std::string_view entity_name,
            const pqxx::unique_violation& ex
        );
    };
}
