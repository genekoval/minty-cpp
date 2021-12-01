#include <minty/conf/settings.test.h>

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <timber/timber>

namespace fs = std::filesystem;

namespace {
    const auto log_path = fs::temp_directory_path() / "minty.test.log";
    const auto settings_path = fs::current_path() / ".test.conf.yaml";

    auto log_file = std::ofstream(log_path);

    auto file_logger(const timber::log& l) noexcept -> void {
        log_file
            << "[" << l.log_level << "] "
            << l.stream.str()
            << std::endl;
    }
}

namespace minty::test {
    auto settings() -> const conf::settings& {
        static auto instance = conf::initialize(settings_path.string());
        return instance;
    }
}

auto main(int argc, char** argv) -> int {
    timber::log_handler = &file_logger;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
