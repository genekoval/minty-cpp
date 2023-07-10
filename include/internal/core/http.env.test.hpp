#pragma once

#include <gtest/gtest.h>
#include <http/http>

class HttpEnvironment : public testing::Environment {
    static HttpEnvironment* instance;

    std::unique_ptr<http::init> init;
public:
    static auto get() -> HttpEnvironment&;

    std::unique_ptr<http::client> client;

    auto SetUp() -> void override;

    auto TearDown() -> void override;
};
