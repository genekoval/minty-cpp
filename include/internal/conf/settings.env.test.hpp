#pragma once

#include "settings.hpp"

#include <gtest/gtest.h>
#include <pg++/pg++>

class SettingsEnvironment : public testing::Environment {
    static SettingsEnvironment* instance;
public:
    static auto get() -> SettingsEnvironment&;

    minty::conf::settings settings;
    pg::parameters db;

    auto SetUp() -> void override;

    auto TearDown() -> void override;
};
