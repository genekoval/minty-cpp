#include "commands.h"
#include "../commands.h"
#include "../../client.h"
#include "../../options/opts.h"
#include "../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto comment(
            const app& app,
            std::optional<std::string_view> path,
            const UUID::uuid& id
        ) -> void {
            auto api = minty::cli::client();

            minty::cli::print_comment(api, id, path);
        }
    }
}

namespace minty::commands {
    auto comment() -> std::unique_ptr<command_node> {
        auto cmd = command(
            __FUNCTION__,
            "Read a comment",
            options(
                cli::opts::path()
            ),
            arguments(
                required<UUID::uuid>("id")
            ),
            internal::comment
        );

        cmd->subcommand(subcommands::comment::add());
        cmd->subcommand(subcommands::comment::edit());

        return cmd;
    }
}
