#pragma once

#include <pqxx/pqxx>
#include <sstream>
#include <type_traits>

namespace minty::repo::db {
    template <
        typename T,
        typename U = std::remove_const_t<T>
    >
    constexpr auto parse_field(
        const pqxx::row& row,
        pqxx::row::size_type index
    ) -> U {
        return row[index].as<std::remove_const_t<T>>();
    }

    class connection_initializer {
        pqxx::connection* connection;
    public:
        connection_initializer(pqxx::connection& c);

        auto prepare(const std::string& name, unsigned int argc) -> void;
    };
}
