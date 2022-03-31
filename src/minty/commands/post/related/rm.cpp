#include "commands.h"

#include "../../../client.h"

using namespace commline;

namespace {
    namespace internal {
        auto rm(
            const app& app,
            std::string_view id,
            const std::vector<std::string_view>& posts
        ) -> void {
            auto api = minty::cli::client();

            for (const auto& post : posts) {
                api.delete_related_post(id, post);
            }
        }
    }
}

namespace minty::subcommands::post_related {
    auto rm() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Remove related posts from a post",
            options(),
            arguments(
                required<std::string_view>("id"),
                variadic<std::string_view>("posts")
            ),
            internal::rm
        );
    }
}
