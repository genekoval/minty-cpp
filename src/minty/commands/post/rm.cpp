#include "commands.h"

#include "../../client.h"

using namespace commline;

namespace {
    namespace internal {
        auto rm(
            const app& app,
            std::string_view id
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
                required<std::string_view>("id")
            ),
            internal::rm
        );
    }
}
