#include "runtime.env.test.hpp"

auto RuntimeEnvironment::SetUp() -> void {
    runtime = std::make_unique<netcore::runtime>();
}
