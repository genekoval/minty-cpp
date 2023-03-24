#include <internal/conf/settings.env.test.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace {
    const auto settings_path = fs::current_path() / TEST_CONFIG;
}

namespace minty::test {
    auto SettingsEnvironment::db_params() -> const pg::parameters& {
        static const auto instance = pg::parameters::parse(
            settings().database.connection.parameters
        );

        return instance;
    }

    auto SettingsEnvironment::settings() -> const conf::settings& {
        static auto instance = conf::initialize(settings_path.string());
        return instance;
    }

    auto SettingsEnvironment::SetUp() -> void {
        settings();
    }
}
