#include "commands.h"

#include "../../../client.h"

using namespace commline;

namespace {
    namespace internal {
        auto rm(
            const app& app,
            std::string_view id,
            const std::vector<std::string_view>& objects
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
                required<std::string_view>("id"),
                variadic<std::string_view>("objects")
            ),
            internal::rm
        );
    }
}
