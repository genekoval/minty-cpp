#include "commands.h"

#include "../../client.h"
#include "../../options/opts.h"
#include "../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto add(
            const app& app,
            std::string title,
            std::string description,
            const std::vector<UUID::uuid>& tags,
            const std::vector<std::string_view>& objects
        ) -> void {
            auto api = minty::cli::client();

            auto parts = minty::core::post_parts {
                .title = title,
                .description = description,
                .tags = tags
            };

            for (const auto& object : objects) {
                parts.objects.emplace_back(
                    minty::cli::add_object(api, object).id
                );
            }

            const auto id = api.add_post(parts);
            std::cout << id << std::endl;
        }
    }
}

namespace minty::subcommands::post {
    auto add() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Add a post",
            options(
                cli::opts::title(),
                cli::opts::description(),
                cli::opts::add_tags()
            ),
            arguments(
                variadic<std::string_view>("objects")
            ),
            internal::add
        );
    }
}
