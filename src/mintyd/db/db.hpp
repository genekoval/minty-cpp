#pragma once

#include <internal/conf/settings.hpp>

#include <dbtools/dbtools>

namespace minty::cli {
    constexpr auto dump_file = "minty.dump";

    auto database(const conf::settings& settings) -> dbtools::postgresql;

    template <typename F>
    requires requires(const F& f, dbtools::postgresql& db) {
        { f(db) } -> std::same_as<ext::task<>>;
    }
    auto database(const conf::settings& settings, const F& action) -> void {
        netcore::run([&]() -> ext::task<> {
            auto db = database(settings);
            co_await action(db);
        }());
    }
}
