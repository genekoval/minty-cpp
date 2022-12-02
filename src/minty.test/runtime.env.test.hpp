#pragma once

#include <gtest/gtest.h>
#include <netcore/netcore>

namespace minty::test {
    class RuntimeEnvironment : public testing::Environment {
        netcore::runtime runtime;
    };
}
