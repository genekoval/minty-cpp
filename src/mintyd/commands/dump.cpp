#include "commands.h"
#include "options/opts.h"
#include "../db/db.h"

using namespace commline;

namespace {
    auto $dump(
        const app& app,
        std::string_view confpath,
        std::optional<std::string_view> filename
    ) -> void {
        const auto settings = minty::conf::initialize(confpath);
        const auto client = minty::cli::data::client(settings);

        client.dump(filename);
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
                opts::config(confpath),
                option<std::optional<std::string_view>>(
                    {"f", "file"},
                    "Send output to the specified file or directory",
                    "filename"
                )
            ),
            arguments(),
            $dump
        );
    }
}
