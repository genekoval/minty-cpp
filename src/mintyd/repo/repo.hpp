#pragma once

#include <internal/conf/settings.hpp>
#include <internal/core/repo.hpp>

namespace minty::cli {
    class container {
        http::client http_client;
        pg::parameters db_params;

        core::db::database database;
        core::object_store objects;
        core::downloader downloader;
        core::search_engine search;
    public:
        core::repo repo;

        container(const minty::conf::settings& settings);

        auto init(const minty::conf::settings& settings) -> ext::task<>;
    };

    template <typename F>
    requires requires(const F& f, core::repo& repo) {
        { f(repo) } -> std::same_as<ext::task<>>;
    }
    auto repo(
        const conf::settings& settings,
        const F& action
    ) -> void {
        netcore::run([&]() -> ext::task<> {
            auto container = cli::container(settings);

            co_await container.init(settings);
            co_await action(container.repo);
        }());
    }
}
