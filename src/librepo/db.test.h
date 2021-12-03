#pragma once

#include <minty/repo/db/db.test.env.h>

#include <fmt/format.h>
#include <gtest/gtest.h>
#include <vector>

class DatabaseTest : public testing::Test {
    pqxx::connection& connection =
        minty::test::DatabaseEnvironment::connection();
protected:
    minty::repo::db::database& database =
        minty::test::DatabaseEnvironment::database();

    virtual auto SetUp() -> void;

    auto count(std::string_view table) -> int;

    template <typename ...Args>
    auto exec0(std::string_view query, Args&&... args) -> void {
        pqxx::nontransaction(connection)
            .exec0(fmt::format(query, args...));
    }

    template <typename ...Args>
    auto exec1(std::string_view query, Args&&... args) -> pqxx::row {
        return pqxx::nontransaction(connection)
            .exec1(fmt::format(query, args...));
    }

    virtual auto tables() -> std::vector<std::string>;

    auto truncate(std::string_view table) -> void;

    auto truncate_tables() -> void;
};
