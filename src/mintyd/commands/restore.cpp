#include "../api/api.h"
#include "commands.h"
#include "../db/db.h"

using namespace commline;

namespace {
    auto $restore(
        const app& app,
        const argv& argv,
        std::string_view confpath,
        std::optional<std::string_view> user
    ) -> void {
        if (argv.empty()) {
            throw std::runtime_error("archive file not specified");
        }

        auto settings = minty::conf::settings::load_file(confpath);
        if (user) settings.database.connection.parameters["user"] = *user;

        const auto client = minty::cli::data::client(settings);

        client.restore(argv.front());
    }
}

namespace minty::cli {
    auto restore(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node> {
        return command(
            __FUNCTION__,
            "Restore a minty database from an archive",
            options(
                option<std::string_view>(
                    {"config", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                ),
                option<std::optional<std::string_view>>(
                    {"user", "u"},
                    "User name to connect as",
                    "username"
                )
            ),
            $restore
        );
    }
}
