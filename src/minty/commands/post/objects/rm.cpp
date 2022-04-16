#include "commands.h"

#include "../../../client.h"
#include "../../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto rm(
            const app& app,
            const UUID::uuid& id,
            const std::vector<UUID::uuid>& objects
        ) -> void {
            auto api = minty::cli::client();
            if (!objects.empty()) api.delete_post_objects(id, objects);
        }
    }
}

namespace minty::subcommands::post_objects {
    auto rm() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Remove objects from a post",
            options(),
            arguments(
                required<UUID::uuid>("id"),
                variadic<UUID::uuid>("objects")
            ),
            internal::rm
        );
    }
}
