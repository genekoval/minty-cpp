#include "../commands.h"
#include "commands.h"

using namespace commline;

namespace minty::cli {
    auto errors(std::string_view confpath) -> std::unique_ptr<command_node> {
        auto cmd = command(
            __FUNCTION__,
            "Inspect various error messages",
            options(),
            arguments(),
            [](const app& app) {}
        );

        cmd->subcommand(sub::errors::preview(confpath));

        return cmd;
    }
}
