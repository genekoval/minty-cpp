#pragma once

#include <ext/coroutine>
#include <string>

namespace minty::core {
    class search_engine;

    class index {
        using config_type = auto() -> std::string;

        config_type* config;
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

        auto clear() -> ext::task<>;

        auto create() -> ext::task<>;

        auto exists() -> ext::task<bool>;

        auto recreate() -> ext::task<>;

        auto refresh() -> ext::task<>;

        auto remove() -> ext::task<>;
    };
}
