#pragma once

#include <minty/repo/db/database.h>

#include <gtest/gtest.h>
#include <vector>

class DatabaseTest : public testing::Test {
    pqxx::connection connection;
protected:
    minty::repo::db::database db;

    DatabaseTest();

    auto truncate(std::string_view table) -> void;
};
