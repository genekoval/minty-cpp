#include <minty/conf/settings.test.env.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace {
    const auto settings_path = fs::current_path() / ".test.conf.yaml";
}

namespace minty::test {
    auto SettingsEnvironment::settings() -> const conf::settings& {
        static auto instance = conf::initialize(settings_path.string());
        return instance;
    }

    auto SettingsEnvironment::SetUp() -> void {
        settings();
    }
}
