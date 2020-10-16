#pragma once

#include <minty/error.h>

#include <optional>
#include <pqxx/pqxx>
#include <sstream>
#include <type_traits>

namespace minty::repo::db {
    class connection_initializer {
        pqxx::connection* connection;
    public:
        connection_initializer(pqxx::connection& c);

        auto prepare(const std::string& name, unsigned int argc) -> void;
    };

    template <pqxx::row::size_type ColumnCount>
    struct entity {
        static constexpr auto column_count = ColumnCount;
    };

    template <typename Entity, typename ...Args>
    auto make_entity(pqxx::transaction_base& tx, Args&&... args) -> Entity {
        const auto row = tx.exec_prepared1(args...);
        auto it = row.begin();
        return Entity(it);
    }

    template <typename>
    struct is_optional : std::false_type {};

    template <typename T>
    struct is_optional<std::optional<T>> : std::true_type {};

    template <typename T>
    inline constexpr auto is_optional_v = is_optional<T>::value;

    using row_iterator = pqxx::row::const_iterator;

    template <
        typename T,
        typename R = std::remove_const_t<T>
    >
    auto read_field(row_iterator& it) -> R {
        return (it++)->as<R>();
    }

    template <typename T>
    auto read_entity(row_iterator& it) -> std::enable_if_t<
        !is_optional_v<T>,
        T
    > {
        return T(it);
    }

    template <typename T>
    auto read_entity(row_iterator& it) -> std::enable_if_t<
        is_optional_v<T>,
        T
    > {
        if (it->is_null()) {
            it += T::value_type::column_count;
            return {};
        }

        return read_entity<T::value_type>(it);
    }

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
