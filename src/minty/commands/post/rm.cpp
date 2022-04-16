#include "commands.h"

#include "../../client.h"
#include "../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto rm(
            const app& app,
            const UUID::uuid& id
        ) -> void {
            auto api = minty::cli::client();
            api.delete_post(id);
        }
    }
}

namespace minty::subcommands::post {
    auto rm() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Remove a post",
            options(),
            arguments(
                required<UUID::uuid>("id")
            ),
            internal::rm
        );
    }
}
