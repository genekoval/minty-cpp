#include "commands.h"
#include "../../api/api.h"
#include "../options/opts.h"

#include <chrono>

using namespace commline;
using namespace std::chrono_literals;

using minty::core::progress;

namespace {
    namespace internal {
        /**
         * One connection for streaming objects and one for logging errors.
         */
        constexpr auto minimum_connections = 2;
        constexpr auto refresh_rate = 500ms;

        auto print_progress(const progress& progress) -> void {
            std::cerr << "\33[2K\033[A\33[2K\r";

            if (progress.total == 0) return;

            const auto percent = static_cast<int>(std::round(
                static_cast<long double>(progress.completed) /
                static_cast<long double>(progress.total) * 100
            ));

            fmt::print(
                stderr,
                "[{}/{}] ({}%)\n",
                progress.completed,
                progress.total,
                percent
            );
        }

        auto run_progress_printer(
            const progress& progress,
            bool& running
        ) -> void {
            while (running) {
                std::this_thread::sleep_for(refresh_rate);
                print_progress(progress);
            }
        }

        auto all(
            const app& app,
            std::string_view confpath,
            int jobs,
            bool quiet
        ) -> void {
            jobs = std::max(jobs, 0);

            auto settings = minty::conf::initialize(confpath);
            settings.database.connections = jobs + minimum_connections;

            auto container = minty::cli::api_container(settings);
            auto& api = container.api();

            auto progress = ::progress();
            auto running = true;

            if (!quiet) fmt::print("\n");

            auto ui_thread = quiet ?
                std::thread() :
                std::thread(
                    run_progress_printer,
                    std::ref(progress),
                    std::ref(running)
                );

            const auto errors = api.regenerate_previews(jobs, progress);

            running = false;
            if (ui_thread.joinable()) ui_thread.join();

            const auto successful = progress.completed - errors;

            fmt::print("Generated previews for {} objects", successful);

            if (errors > 0) {
                fmt::print(
                    " with {} failures\n"
                    "Run '{} errors preview' for more information",
                    errors,
                    app.argv0
                );
            }

            fmt::print("\n");
        }
    }
}

namespace minty::cli::sub::regen {
    auto all(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Regenerate all object previews",
            options(
                opts::config(confpath),
                option<int>(
                    {"j", "jobs"},
                    "Number of additional jobs to run simultaneously",
                    "jobs",
                    0
                ),
                flag({"q", "quiet"}, "Do not print progress")
            ),
            arguments(),
            internal::all
        );
    }
}
