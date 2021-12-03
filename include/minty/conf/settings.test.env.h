#pragma once

#include <minty/conf/settings.h>

#include <gtest/gtest.h>

namespace minty::test {
    struct SettingsEnvironment : testing::Environment {
        static auto settings() -> const conf::settings&;

        virtual auto SetUp() -> void override;
    };
}
