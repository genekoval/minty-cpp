#include "commands.h"

#include "../../../client.h"
#include "../../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto add(
            const app& app,
            std::int16_t index,
            const UUID::uuid& id,
            const std::vector<std::string_view>& objects
        ) -> void {
            auto api = minty::cli::client();

            auto uploaded = std::vector<UUID::uuid>();
            const auto object_previews = api.add_objects(objects);
            std::ranges::transform(
                object_previews,
                std::back_inserter(uploaded),
                [](const auto& obj) -> UUID::uuid { return obj.id; }
            );

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
                required<UUID::uuid>("id"),
                variadic<std::string_view>("objects")
            ),
            internal::add
        );
    }
}
