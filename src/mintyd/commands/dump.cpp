#include "commands.h"
#include "../db/db.h"

using namespace commline;

namespace {
    auto $dump(
        const app& app,
        const argv& argv,
        std::string_view confpath,
        std::optional<std::string_view> file
    ) -> void {
        const auto settings = minty::conf::settings::load_file(confpath);
        const auto client = minty::cli::data::client(settings);

        client.dump(file);
    }
}

namespace minty::cli {
    auto dump(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Create a backup of the database",
            options(
                option<std::string_view>(
                    {"config", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                ),
                option<std::optional<std::string_view>>(
                    {"file", "f"},
                    "Send output to the specified file or directory",
                    "file"
                )
            ),
            $dump
        );
    }
}
