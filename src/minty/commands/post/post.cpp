#include "commands.h"

#include "../commands.h"
#include "../../client.h"
#include "../../options/opts.h"
#include "../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto post(
            const app& app,
            std::optional<std::string_view> path,
            const UUID::uuid& id
        ) -> void {
            auto api = minty::cli::client();

            minty::cli::print_post(api, id, path);
        }
    }
}

namespace minty::commands {
    auto post() -> std::unique_ptr<command_node> {
        auto cmd = command(
            __FUNCTION__,
            "View or edit a post",
            options(
                cli::opts::path()
            ),
            arguments(
                required<UUID::uuid>("id")
            ),
            internal::post
        );

        cmd->subcommand(subcommands::post::add());
        cmd->subcommand(subcommands::post::comments());
        cmd->subcommand(subcommands::post::description());
        cmd->subcommand(subcommands::post::find());
        cmd->subcommand(subcommands::post::objects());
        cmd->subcommand(subcommands::post::related());
        cmd->subcommand(subcommands::post::rm());
        cmd->subcommand(subcommands::post::tags());
        cmd->subcommand(subcommands::post::title());

        return cmd;
    }
}
