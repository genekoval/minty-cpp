#include "commands.h"
#include "options/opts.h"
#include "../db/db.h"

using namespace commline;

namespace {
    auto $db(
        const app& app,
        std::string_view confpath,
        const std::vector<std::string_view>& command
    ) -> void {
        const auto db = minty::cli::database(confpath);
        db.exec(command);
    }
}

namespace minty::cli {
    auto db(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "db",
            "Connect to the database using the psql client",
            options(
                opts::config(confpath)
            ),
            arguments(
                variadic<std::string_view>("command")
            ),
            $db
        );
    }
}
