#include "Database.test.hpp"

#include <internal/conf/settings.env.test.hpp>

#include <fmt/format.h>

namespace {
    auto connect() -> ext::task<pg::client> {
        co_return co_await pg::connect(SettingsEnvironment::get().db);
    }

    auto database() -> minty::core::db::database {
        return minty::core::db::database(SettingsEnvironment::get().db);
    }
}

auto DatabaseTest::count(std::string_view table) -> ext::task<std::int64_t> {
    co_return co_await client->fetch<std::int64_t>(fmt::format(
        "SELECT count(*) FROM data.{}",
        table
    ));
}

auto DatabaseTest::run(ext::task<>&& task) -> void {
    netcore::run(run_async(std::forward<ext::task<>>(task)));
}

auto DatabaseTest::run_async(ext::task<>&& task) -> ext::task<> {
    auto client = co_await connect();
    this->client = &client;

    auto db = database();
    auto connection = co_await db.connect();
    this->db = &connection;

    co_await truncate_tables();

    co_await std::forward<ext::task<>>(task);

    this->client = nullptr;
    this->db = nullptr;
}

auto DatabaseTest::truncate(std::string_view table) -> ext::task<> {
    co_await client->exec(fmt::format("TRUNCATE data.{} CASCADE", table));
}

auto DatabaseTest::truncate_tables() -> ext::task<> {
    for (const auto& table : tables()) co_await truncate(table);
}
