#include "commands.h"

#include "../commands.h"
#include "../../../client.h"
#include "../../../output.h"

using namespace commline;

namespace {
    namespace internal {
        auto related(
            const app& app,
            std::string_view id
        ) -> void {
            auto api = minty::cli::client();

            const auto post = api.get_post(id);

            minty::cli::print(post.posts);
        }
    }
}

namespace minty::subcommands::post {
    auto related() -> std::unique_ptr<command_node> {
        auto cmd = command(
            __FUNCTION__,
            "Get a post's related posts",
            options(),
            arguments(
                required<std::string_view>("id")
            ),
            internal::related
        );

        cmd->subcommand(post_related::add());
        cmd->subcommand(post_related::rm());

        return cmd;
    }
}
