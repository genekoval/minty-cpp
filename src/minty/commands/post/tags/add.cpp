#include "commands.h"

#include "../../../client.h"

using namespace commline;

namespace {
    namespace internal {
        auto add(
            const app& app,
            std::string_view id,
            const std::vector<std::string_view>& tags
        ) -> void {
            auto api = minty::cli::client();

            for (const auto& tag : tags) {
                api.add_post_tag(id, tag);
            }
        }
    }
}

namespace minty::subcommands::post_tags {
    auto add() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Add tags to a post",
            options(),
            arguments(
                required<std::string_view>("id"),
                variadic<std::string_view>("tags")
            ),
            internal::add
        );
    }
}
