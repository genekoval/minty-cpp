#pragma once

#include <minty/repo/db/database.h>

#include <gtest/gtest.h>
#include <vector>

class DatabaseTest : public testing::Test {
    pqxx::connection connection;
protected:
    minty::repo::db::database database;

    DatabaseTest();

    virtual auto SetUp() -> void;

    virtual auto TearDown() -> void;

    virtual auto tables() -> std::vector<std::string>;

    auto truncate(std::string_view table) -> void;

    auto truncate_tables() -> void;
};
