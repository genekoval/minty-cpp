#pragma once

#include <internal/conf/settings.hpp>
#include <internal/core/api.hpp>

namespace minty::cli {
    class api_container {
        minty::core::db::database db;
        minty::core::object_store objects;
        minty::core::downloader downloader;
        minty::core::search_engine search;
    public:
        minty::core::api api;

        api_container(const minty::conf::settings& settings);

        auto init(const minty::conf::settings& settings) -> ext::task<>;
    };

    template <typename F>
    requires requires(const F& f, core::api& api) {
        { f(api) } -> std::same_as<ext::task<>>;
    }
    auto api(
        const conf::settings& settings,
        const F& action
    ) -> void {
        netcore::async([&]() -> ext::task<> {
            auto container = api_container(settings);

            co_await container.init(settings);
            co_await action(container.api);
        }());
    }
}
