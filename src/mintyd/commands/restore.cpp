#include "commands.h"
#include "options/opts.h"
#include "../api/api.h"
#include "../db/db.hpp"

using namespace commline;

namespace {
    namespace internal {
        auto restore(
            const app& app,
            std::string_view confpath,
            std::optional<std::string_view> user,
            std::string_view filename
        ) -> void {
            auto settings = minty::conf::initialize(confpath);
            if (user) settings.database.connection.parameters["user"] = *user;

            minty::cli::database(settings, [filename](
                dbtools::postgresql& db
            ) -> ext::task<> {
                co_await db.restore(filename);
            });
        }
    }
}

namespace minty::cli {
    auto restore(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Restore a minty database from an archive",
            options(
                opts::config(confpath),
                option<std::optional<std::string_view>>(
                    {"u", "user"},
                    "User name to connect as",
                    "username"
                )
            ),
            arguments(
                required<std::string_view>("filename")
            ),
            internal::restore
        );
    }
}
