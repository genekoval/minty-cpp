#include "runtime.env.test.hpp"

#include <internal/core/db/db.env.test.hpp>
#include <internal/conf/settings.env.test.hpp>
#include <internal/core/search/search.env.test.hpp>

#include <filesystem>
#include <fmt/chrono.h>
#include <fmt/os.h>
#include <gtest/gtest.h>
#include <timber/timber>

namespace fs = std::filesystem;

using namespace std::literals;

using testing::AddGlobalTestEnvironment;
using testing::EmptyTestEventListener;
using testing::InitGoogleTest;
using testing::TestInfo;
using testing::TestPartResult;
using testing::UnitTest;

namespace {
    const auto log_path = fs::temp_directory_path() / "minty.test.log";
    auto log_file = fmt::output_file(log_path.native());

    auto file_logger(const timber::log& log) noexcept -> void {
        log_file.print("{:%b %d %r}", log.timestamp);
        log_file.print(" {:>9}  ", log.log_level);
        log_file.print("{}\n", log.message);

        log_file.flush();
    }
}

struct TestLogger : public EmptyTestEventListener {
    auto OnTestStart(const TestInfo& info) -> void override {
        TIMBER_NOTICE(
            "Test {}.{} starting",
            info.test_suite_name(),
            info.name()
        );
    }

    auto OnTestPartResult(const TestPartResult& result) -> void override {
        TIMBER_INFO(
            "{} in {} {}",
            result.failed() ? "Failure"sv : "Success"sv,
            result.file_name() ?
                fmt::format(
                    "{}:{}",
                    result.file_name(),
                    result.line_number()
                ) : "unknown file"s,
            result.summary()
        );
    }

    auto OnTestEnd(const TestInfo& info) -> void override {
        TIMBER_NOTICE(
            "Test {}.{} ending",
            info.test_suite_name(),
            info.name()
        );
    }
};

auto main(int argc, char** argv) -> int {
    timber::log_handler = &file_logger;

    const auto client = http::init();

    InitGoogleTest(&argc, argv);

    auto& listeners = UnitTest::GetInstance()->listeners();
    listeners.Append(new TestLogger);

    AddGlobalTestEnvironment(new minty::test::RuntimeEnvironment);
    AddGlobalTestEnvironment(new minty::test::SearchEnvironment);
    AddGlobalTestEnvironment(new minty::test::SettingsEnvironment);
    AddGlobalTestEnvironment(new minty::test::DatabaseEnvironment);

    return RUN_ALL_TESTS();
}
