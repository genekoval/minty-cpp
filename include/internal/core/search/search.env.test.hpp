#pragma once

#include "search.hpp"

#include <gtest/gtest.h>

namespace minty::test {
    struct SearchEnvironment : testing::Environment {
        static auto client() -> elastic::elasticsearch&;

        static auto engine() -> core::search_engine&;

        virtual auto SetUp() -> void override;
    };
}
