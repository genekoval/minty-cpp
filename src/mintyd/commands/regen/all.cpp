#include "../../repo/repo.hpp"
#include "../options/opts.h"
#include "commands.h"

#include <chrono>

using namespace commline;
using namespace std::chrono_literals;

using ext::jtask;
using minty::core::progress;

namespace {
    namespace internal {
        constexpr auto refresh_rate = 500ms;

        auto print_progress(const progress& progress) -> void {
            fmt::print(stderr, "\33[2K\033[A\33[2K\r");

            if (progress.total == 0) return;

            const auto percent = static_cast<int>(std::round(
                static_cast<long double>(progress.completed) /
                static_cast<long double>(progress.total) * 100
            ));

            fmt::print(
                stderr,
                "[{:L}/{:L}] ({}%)\n",
                progress.completed,
                progress.total,
                percent
            );
        }

        auto run_progress_printer(const progress& progress, bool& running)
            -> jtask<> {
            while (running) {
                co_await netcore::sleep_for(refresh_rate);
                print_progress(progress);
            }
        }

        auto all(
            const app& app,
            std::string_view confpath,
            unsigned int batch_size,
            unsigned int jobs,
            bool quiet
        ) -> void {
            jobs = std::max(jobs, 1u);

            auto settings = minty::conf::initialize(confpath);

            auto progress = ::progress();
            size_t errors = 0;

            minty::cli::repo(
                settings,
                [&](minty::core::repo& repo) -> ext::task<> {
                    if (!quiet) fmt::print("\n");

                    auto running = true;
                    auto printer =
                        quiet ? jtask()
                              : run_progress_printer(progress, running);

                    errors = co_await repo.regenerate_previews(
                        batch_size,
                        jobs,
                        progress
                    );

                    running = false;
                    if (printer.joinable()) co_await printer;
                }
            );

            if (progress.total == 0) {
                fmt::print("No objects to process\n");
                return;
            }

            fmt::print(
                "Processed {:L} object{}",
                progress.completed,
                progress.completed == 1 ? "" : "s"
            );

            if (errors > 0) {
                fmt::print(
                    " with {:L} failure{}\n"
                    "Run '{} errors preview' for more information\n",
                    errors,
                    errors == 1 ? "" : "s",
                    app.argv0
                );
            }
            else fmt::print("\n");
        }
    }
}

namespace minty::cli::sub::regen {
    auto all(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Regenerate all object previews",
            options(
                opts::config(confpath),
                option<unsigned int>(
                    {"b", "batch-size"},
                    "Number of objects to process at a time",
                    "size",
                    100
                ),
                option<unsigned int>(
                    {"j", "jobs"},
                    "Number of additional jobs to run simultaneously",
                    "jobs",
                    std::thread::hardware_concurrency()
                ),
                flag({"q", "quiet"}, "Do not print progress")
            ),
            arguments(),
            internal::all
        );
    }
}
