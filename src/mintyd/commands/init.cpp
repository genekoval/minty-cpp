#include "commands.h"
#include "options/opts.h"
#include "../db/db.hpp"
#include "../repo/repo.hpp"

using namespace commline;

namespace {
    namespace internal {
        auto init(
            const app& app,
            std::string_view confpath,
            bool skip_index
        ) -> void {
            const auto settings = minty::conf::initialize(confpath);

            minty::cli::database(settings, [&app](
                dbtools::postgresql& db
            ) -> ext::task<> {
                co_await db.init(app.version);
            });

            if (skip_index) return;

            minty::cli::repo(
                settings,
                [](minty::core::repo& repo) -> ext::task<> {
                    co_await repo.reindex();
                }
            );
        }
    }
}

namespace minty::cli {
    auto init(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "init",
            "Initialize the database",
            options(
                opts::config(confpath),
                flag({"skip-index"}, "Skip search engine index operation")
            ),
            arguments(),
            internal::init
        );
    }
}
