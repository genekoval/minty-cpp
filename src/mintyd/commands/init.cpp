#include "../db/db.hpp"
#include "../repo/repo.hpp"
#include "commands.h"
#include "options/opts.h"

using namespace commline;

namespace {
    namespace internal {
        auto init(const app& app, std::string_view confpath, bool overwrite)
            -> void {
            const auto settings = minty::conf::initialize(confpath);

            minty::cli::database(
                settings,
                [&app, overwrite](dbtools::postgresql& db) -> ext::task<> {
                    if (overwrite) co_await db.reset(app.version);
                    else co_await db.init(app.version);
                }
            );

            minty::cli::repo(
                settings,
                [](minty::core::repo& repo) -> ext::task<> {
                    co_await repo.create_indices();
                }
            );
        }
    }
}

namespace minty::cli {
    auto init(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            "init",
            "Initialize the database",
            options(
                opts::config(confpath),
                flag({"o", "overwrite"}, "Delete existing data if necessary")
            ),
            arguments(),
            internal::init
        );
    }
}
