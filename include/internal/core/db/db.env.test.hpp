#pragma once

#include "database.hpp"

#include <gtest/gtest.h>

namespace minty::test {
    struct DatabaseEnvironment : testing::Environment {
        static auto connection() -> pqxx::connection&;

        static auto database() -> core::db::database&;

        virtual auto SetUp() -> void override;
    };
}
