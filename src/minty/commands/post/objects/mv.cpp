#include "commands.h"

#include "../../../client.h"

using namespace commline;

namespace {
    namespace internal {
        auto mv(
            const app& app,
            std::optional<std::string_view> destination,
            std::string_view id,
            const std::vector<std::string_view>& objects
        ) -> void {
            if (objects.empty()) return;

            auto api = minty::cli::client();
            api.move_post_objects(id, objects, destination);
        }
    }
}

namespace minty::subcommands::post_objects {
    auto mv() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Reorder objects within a post.",
            options(
                option<std::optional<std::string_view>>(
                    {"d", "destination"},
                    "Move OBJECTS in front of this object",
                    "object"
                )
            ),
            arguments(
                required<std::string_view>("id"),
                variadic<std::string_view>("objects")
            ),
            internal::mv
        );
    }
}
