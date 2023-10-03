#include "../db/db.hpp"
#include "commands.h"
#include "options/opts.h"

using namespace commline;

namespace {
    namespace internal {
        auto dump(
            const app& app,
            std::string_view confpath,
            std::string_view filename
        ) -> void {
            const auto settings = minty::conf::initialize(confpath);

            minty::cli::database(
                settings,
                [filename](dbtools::postgresql& db) -> ext::task<> {
                    co_await db.dump(filename);
                }
            );
        }
    }
}

namespace minty::cli {
    auto dump(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Create a backup of the database",
            options(
                opts::config(confpath),
                option<std::string_view>(
                    {"f", "file"},
                    "Send output to the specified file or directory",
                    "filename",
                    dump_file
                )
            ),
            arguments(),
            internal::dump
        );
    }
}
