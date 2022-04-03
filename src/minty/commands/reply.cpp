#include "commands.h"

#include "../client.h"
#include "../output.h"

using namespace commline;

namespace {
    namespace internal {
        auto reply(
            const app& app,
            std::string_view comment,
            std::string_view content
        ) -> void {
            auto api = minty::cli::client();

            const auto result = api.add_reply(comment, content);

            std::cout << result.id << std::endl;
        }
    }
}

namespace minty::commands {
    auto reply() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Create a reply to a comment",
            options(),
            arguments(
                required<std::string_view>("comment"),
                required<std::string_view>("content")
            ),
            internal::reply
        );
    }
}
