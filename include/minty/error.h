#pragma once

#include <ext/string.h>
#include <fmt/format.h>
#include <stdexcept>

namespace minty {
    class minty_error : public std::runtime_error {
    public:
        template <typename... Args>
        minty_error(std::string_view format_str, Args&&... args) :
            std::runtime_error(fmt::format(
                fmt::runtime(format_str),
                std::forward<Args>(args)...
            ))
        {}
    };

    class unique_entity_violation : public minty_error {
    public:
        const std::string entity;
        const std::string key;
        const std::string value;

        unique_entity_violation(
            std::string_view entity,
            std::string_view key,
            std::string_view value
        ) :
            minty_error(fmt::format(
                "{} with {} {} already exists",
                entity,
                key,
                ext::quote(value)
            )),
            entity(entity),
            key(key),
            value(value)
        {}

    };
}
