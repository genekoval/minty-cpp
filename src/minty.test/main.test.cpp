#include <minty/repo/db/db.env.test.h>
#include <minty/conf/settings.env.test.h>
#include <minty/core/search/search.env.test.h>

#include <filesystem>
#include <fmt/chrono.h>
#include <fmt/os.h>
#include <gtest/gtest.h>
#include <timber/timber>

namespace fs = std::filesystem;

namespace {
    const auto log_path = fs::temp_directory_path() / "minty.test.log";
    auto log_file = fmt::output_file(log_path.native());

    auto file_logger(const timber::log& log) noexcept -> void {
        log_file.print("{:%b %d %r}", log.timestamp);
        log_file.print(" {:>9}  ", log.log_level);
        log_file.print("{}\n", log.message);
    }
}

auto main(int argc, char** argv) -> int {
    timber::log_handler = &file_logger;

    const auto client = http::init();

    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new minty::test::SearchEnvironment);
    testing::AddGlobalTestEnvironment(new minty::test::SettingsEnvironment);
    testing::AddGlobalTestEnvironment(new minty::test::DatabaseEnvironment);

    return RUN_ALL_TESTS();
}
