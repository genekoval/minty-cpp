#pragma once

#include "search.hpp"

#include <internal/conf/settings.hpp>

#include <gtest/gtest.h>

class SearchEnvironment : public testing::Environment {
    static SearchEnvironment* instance;
public:
    static auto get() -> SearchEnvironment&;

    std::unique_ptr<elastic::elasticsearch> client;
    std::unique_ptr<minty::core::search_engine> engine;

    auto SetUp() -> void override;

    auto TearDown() -> void override;
};
