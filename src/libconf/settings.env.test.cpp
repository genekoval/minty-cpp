#include <internal/conf/settings.env.test.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace {
    const auto settings_path = fs::current_path() / TEST_CONFIG;
}

SettingsEnvironment* SettingsEnvironment::instance = nullptr;

auto SettingsEnvironment::get() -> SettingsEnvironment& {
    if (instance) return *instance;
    throw std::runtime_error("Settings Environment instance is missing");
}

auto SettingsEnvironment::SetUp() -> void {
    settings = minty::conf::initialize(settings_path.string());
    db = pg::parameters::parse(settings.database.connection.parameters);

    instance = this;
}

auto SettingsEnvironment::TearDown() -> void { instance = nullptr; }
