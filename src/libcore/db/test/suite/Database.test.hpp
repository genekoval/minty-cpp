#pragma once

#include <internal/core/db/database.hpp>

#include <gtest/gtest.h>

class DatabaseTest : public testing::Test {
    auto run_async(ext::task<>&& task) -> ext::task<>;

    auto truncate(std::string_view table) -> ext::task<>;

    auto truncate_tables() -> ext::task<>;
protected:
    pg::client* client = nullptr;
    minty::core::db::connection_wrapper* db = nullptr;

    auto count(std::string_view table) -> ext::task<std::int64_t>;

    auto run(ext::task<>&& task) -> void;

    virtual auto tables() -> std::vector<std::string> = 0;
};
