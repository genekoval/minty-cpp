#include "commands.h"

#include "../../client.h"

using namespace commline;

namespace {
    namespace internal {
        auto description(
            const app& app,
            std::string_view id,
            std::optional<std::string_view> description
        ) -> void {
            auto api = minty::cli::client();

            if (description) api.set_post_description(id, *description);

            const auto post = api.get_post(id);
            if (post.description) std::cout << *post.description << std::endl;
        }
    }
}

namespace minty::subcommands::post {
    auto description() -> std::unique_ptr<commline::command_node> {
        return command(
            __FUNCTION__,
            "Get or set a post's description",
            options(),
            arguments(
                required<std::string_view>("id"),
                optional<std::string_view>("description")
            ),
            internal::description
        );
    }
}
