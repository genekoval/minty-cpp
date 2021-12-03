#pragma once

#include <minty/repo/db/database.h>

#include <gtest/gtest.h>

namespace minty::test {
    struct DatabaseEnvironment : testing::Environment {
        static auto connection() -> pqxx::connection&;

        static auto database() -> repo::db::database&;

        virtual auto SetUp() -> void override;
    };
}
