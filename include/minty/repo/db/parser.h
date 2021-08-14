#pragma once

#include "model.h"

#include <pqxx/pqxx>
#include <timber/timber>

#define ENTITY(Entity) \
    template <> \
    struct field_parser<Entity> { \
        static auto read(row_iterator& it) -> Entity; \
    };

#define READ_ENTITY(Entity, ...) \
    auto field_parser<Entity>::read(row_iterator& it) -> Entity {\
        return read_entity<Entity>(it, __VA_ARGS__); \
    }

namespace minty::repo::db {
    using row_iterator = pqxx::row::const_iterator;
    using transaction = pqxx::transaction_base;

    template <typename T>
    auto read_field(row_iterator& it) -> T {
        return (it++)->as<T>();
    }

    template <typename T>
    struct field_parser {
        static auto read(row_iterator& it) -> T {
            return read_field<T>(it);
        }
    };

    template <typename T>
    struct field_parser<std::optional<T>> {
        static auto read(row_iterator& it) -> std::optional<T> {
            if constexpr (std::is_base_of_v<entity_base, T>) {
                if (it.is_null()) {
                    it += T::column_count;
                    return {};
                }

                return field_parser<T>::read(it);
            }

            return read_field<std::optional<T>>(it);
        }
    };

    template <typename T>
    struct field_parser<std::vector<T>> {
        static auto read(row_iterator& it) -> std::vector<T> {
            using juncture = pqxx::array_parser::juncture;

            auto array = (it++)->as_array();
            auto result = std::vector<T>();

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
                            << static_cast<std::underlying_type_t<juncture>>(
                                junct
                            );
                }
            }

            return result;
        }
    };

    template <typename T, typename Type>
    auto read_entity_member(
        row_iterator& it,
        T& t,
        Type T::* member
    ) -> void {
        (t.*member) = field_parser<Type>::read(it);
    }

    template <typename T, typename ...Members>
    auto read_entity(row_iterator& it, Members... members) -> T {
        auto t = T();
        (read_entity_member(it, t, members), ...);
        return t;
    }

    template <typename Entity, typename ...Args>
    auto make_entity(transaction& tx, Args&&... args) -> Entity {
        const auto row = tx.exec_prepared1(args...);
        auto it = row.begin();
        return field_parser<Entity>::read(it);
    }

    template <typename Collection, typename ...Args>
    auto read_rows(
        transaction& tx,
        typename Collection::value_type (*handle_row)(const pqxx::row&),
        Args&&... args
    ) -> Collection {
        const auto rows = tx.exec_prepared(args...);
        auto result = Collection();

        for (const auto& row : rows) {
            result.push_back(handle_row(row));
        }

        return result;
    }

    template <typename Collection, typename ...Args>
    auto make_entities(transaction& tx, Args&&... args) -> Collection {
        using Entity = typename Collection::value_type;

        return read_rows<Collection>(
            tx,
            [](const auto& row) -> Entity {
                auto it = row.begin();
                return field_parser<Entity>::read(it);
            },
            std::forward<Args>(args)...
        );
    }

    template <typename Collection, typename ...Args>
    auto make_objects(transaction& tx, Args&&... args) -> Collection {
        using Object = typename Collection::value_type;

        return read_rows<Collection>(
            tx,
            [](const auto& row) -> Object {
                return row[0].template as<Object>();
            },
            std::forward<Args>(args)...
        );
    }
}
