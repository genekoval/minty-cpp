#include "commands.h"

#include "../commands.h"
#include "../../../client.h"
#include "../../../output.h"

using namespace commline;

namespace {
    namespace internal {
        auto tags(
            const app& app,
            std::string_view id
        ) -> void {
            auto api = minty::cli::client();

            const auto post = api.get_post(id);

            minty::cli::print(post.tags);
        }
    }
}

namespace minty::subcommands::post {
    auto tags() -> std::unique_ptr<command_node> {
        auto cmd = command(
            __FUNCTION__,
            "Get a post's tags",
            options(),
            arguments(
                required<std::string_view>("id")
            ),
            internal::tags
        );

        cmd->subcommand(post_tags::add());
        cmd->subcommand(post_tags::rm());

        return cmd;
    }
}
