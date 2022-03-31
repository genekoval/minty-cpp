#include "commands.h"

#include "../../../client.h"

using namespace commline;

namespace {
    namespace internal {
        auto add(
            const app& app,
            std::int16_t index,
            std::string_view id,
            const std::vector<std::string_view>& objects
        ) -> void {
            auto api = minty::cli::client();

            auto uploaded = std::vector<std::string>();

            for (const auto& object : objects) {
                uploaded.push_back(minty::cli::add_object(api, object));
            }

            if (!uploaded.empty()) api.add_post_objects(id, uploaded, index);
        }
    }
}

namespace minty::subcommands::post_objects {
    auto add() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Add objects to a post",
            options(
                option<std::int16_t>(
                    {"i", "index"},
                    "Index of where to add the objects",
                    "number",
                    -1 // Append to the end.
                )
            ),
            arguments(
                required<std::string_view>("id"),
                variadic<std::string_view>("objects")
            ),
            internal::add
        );
    }
}
