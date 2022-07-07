#pragma once

#include <string>

namespace minty::core {
    class search_engine;

    class index {
        using config_type = auto () -> std::string;

        const config_type* config;
        search_engine* const search;
    public:
        const std::string name;

        index();

        index(
            search_engine* search,
            std::string_view ns,
            std::string_view name,
            config_type* config
        );

        operator std::string_view() const noexcept;

        auto clear() -> void;

        auto create() -> void;

        auto exists() -> bool;

        auto refresh() -> void;

        auto remove() -> void;
    };
}
