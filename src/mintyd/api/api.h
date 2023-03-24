#pragma once

#include <internal/conf/settings.hpp>
#include <internal/core/api.hpp>

namespace minty::cli {
    class api_container {
        pg::parameters db_params;

        core::db::database database;
        core::object_store objects;
        core::downloader downloader;
        core::search_engine search;
    public:
        core::api api;

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
