#pragma once

#include <gtest/gtest.h>
#include <netcore/netcore>

class RuntimeEnvironment : public testing::Environment {
    std::unique_ptr<netcore::runtime> runtime;
public:
    auto SetUp() -> void override;
};
