#pragma once

#include <pqxx/pqxx>
#include <type_traits>

namespace minty::repo::db {
    template <
        pqxx::row::size_type I,
        typename T,
        typename U = std::remove_const_t<T>
    >
    constexpr auto parse_field(const pqxx::row& row) -> U {
        return row[I].as<std::remove_const_t<T>>();
    }
}
