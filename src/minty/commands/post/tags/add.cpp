#include "commands.h"

#include "../../../client.h"
#include "../../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto add(
            const app& app,
            const UUID::uuid& id,
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
                required<UUID::uuid>("id"),
                variadic<std::string_view>("tags")
            ),
            internal::add
        );
    }
}
