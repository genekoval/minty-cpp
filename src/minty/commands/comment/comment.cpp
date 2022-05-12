#include "commands.h"
#include "../commands.h"
#include "../../client.h"
#include "../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto comment(
            const app& app,
            const UUID::uuid& id
        ) -> void {
            // TODO Implement ability to fetch a single comment.
            throw std::runtime_error("not implemented");
        }
    }
}

namespace minty::commands {
    auto comment() -> std::unique_ptr<command_node> {
        auto cmd = command(
            __FUNCTION__,
            "Read a comment",
            options(),
            arguments(
                required<UUID::uuid>("id")
            ),
            internal::comment
        );

        cmd->subcommand(subcommands::comment::add());

        return cmd;
    }
}
