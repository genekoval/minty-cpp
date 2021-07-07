#pragma once

#include <minty/error.h>

#include <optional>
#include <pqxx/pqxx>
#include <sstream>
#include <timber/timber>
#include <type_traits>

namespace minty::repo::db {
    using row_iterator = pqxx::row::const_iterator;
    using transaction = pqxx::transaction_base;

    template <typename Model>
    struct parser {};

    class connection_initializer {
        pqxx::connection* connection;
    public:
        connection_initializer(pqxx::connection& c);

        auto prepare(const std::string& name, unsigned int argc) -> void;
    };

    template <typename Entity, typename ...Args>
    auto make_entity(transaction& tx, Args&&... args) -> Entity {
        const auto row = tx.exec_prepared1(args...);
        auto it = row.begin();
        return parser<Entity>::read(it, tx);
    }

    template <typename Collection, typename ...Args>
    auto make_entities(transaction& tx, Args&&... args) -> Collection {
        using Entity = typename Collection::value_type;

        const auto rows = tx.exec_prepared(args...);
        auto result = Collection();

        for (const auto& row : rows) {
            auto it = row.begin();
            result.push_back(parser<Entity>::read(it, tx));
        }

        return result;
    }

    template <typename>
    struct is_optional : std::false_type {};

    template <typename T>
    struct is_optional<std::optional<T>> : std::true_type {};

    template <typename T>
    inline constexpr auto is_optional_v = is_optional<T>::value;

    template <
        typename T,
        typename R = std::remove_const_t<T>
    >
    auto read_array(row_iterator& it) -> R {
        using juncture = pqxx::array_parser::juncture;

        auto array = (it++)->as_array();
        auto result = R();

        auto parsing = true;
        while (parsing) {
            auto [junct, value] = array.get_next();

            switch (junct) {
                case juncture::string_value:
                    result.push_back(value);
                    break;
                case juncture::done:
                    parsing = false;
                    break;
                case juncture::row_start:
                case juncture::row_end:
                    // Do nothing.
                    // We currently do not use nested arrays.
                    break;
                default:
                    WARNING()
                        << "Unhandled array parser juncture: "
                        << static_cast<int>(junct);
            }
        }

        return result;
    }

    template <
        typename T,
        typename R = std::remove_const_t<T>
    >
    auto read_field(row_iterator& it) -> R {
        return (it++)->as<R>();
    }

    template <typename T>
    auto read_entity(row_iterator& it, transaction& tx) -> std::enable_if_t<
        not is_optional_v<std::remove_const_t<T>>,
        T
    > {
        return parser<T>::read(it, tx);
    }

    template <typename T>
    auto read_entity(row_iterator& it, transaction& tx) -> std::enable_if_t<
        is_optional_v<std::remove_const_t<T>>,
        T
    > {
        if (it->is_null()) {
            it += T::value_type::column_count;
            return {};
        }

        return read_entity<typename T::value_type>(it, tx);
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
