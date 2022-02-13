#include "commands.h"
#include "db/commands.h"
#include "options/opts.h"
#include "../db/db.h"

using namespace commline;

namespace {
    auto $db(
        const app& app,
        std::string_view confpath,
        const std::vector<std::string_view>& command
    ) -> void {
        const auto settings = minty::conf::initialize(confpath);
        const auto client = minty::cli::data::client(settings);

        client.exec(command);
    }
}

namespace minty::cli {
    auto db(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        auto cmd = command(
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

        cmd->subcommand(init(confpath));
        cmd->subcommand(migrate(confpath));

        return cmd;
    }
}
