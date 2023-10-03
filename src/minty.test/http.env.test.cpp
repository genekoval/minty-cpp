#include <internal/core/http.env.test.hpp>

HttpEnvironment* HttpEnvironment::instance = nullptr;

auto HttpEnvironment::get() -> HttpEnvironment& {
    if (instance) return *instance;
    throw std::runtime_error("HTTP Environment instance is missing");
}

auto HttpEnvironment::SetUp() -> void {
    init = std::make_unique<http::init>();
    session = std::make_unique<http::session>();

    instance = this;
}

auto HttpEnvironment::TearDown() -> void { instance = nullptr; }
