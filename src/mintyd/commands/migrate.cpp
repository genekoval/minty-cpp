#include "commands.h"
#include "options/opts.h"
#include "../db/db.hpp"

using namespace commline;

namespace {
    namespace internal {
        auto migrate(
            const app& app,
            std::string_view confpath
        ) -> void {
            const auto settings = minty::conf::initialize(confpath);

            minty::cli::database(settings, [&app](
                dbtools::postgresql& db
            ) -> ext::task<> {
                co_await db.migrate(app.version);
            });
        }
    }
}

namespace minty::cli {
    auto migrate(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Update schemas to the current program version",
            options(
                opts::config(confpath)
            ),
            arguments(),
            internal::migrate
        );
    }
}
