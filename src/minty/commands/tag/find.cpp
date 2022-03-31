#include "commands.h"

#include "../../client.h"
#include "../../options/opts.h"
#include "../../output.h"

using namespace commline;

namespace {
    auto $find(
        const commline::app& app,
        unsigned int from,
        unsigned int size,
        std::optional<std::string_view> path,
        const std::string& name
    ) -> void {
        const auto query = minty::core::tag_query {
            .from = from,
            .size = size,
            .name = name
        };

        auto api = minty::cli::client();
        minty::cli::print(api.get_tags(query), path);
    }
}

namespace minty::subcommands::tag {
    auto find() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Find tags",
            options(
                cli::opts::from(),
                cli::opts::size(),
                cli::opts::path()
            ),
            arguments(
                required<std::string>("name")
            ),
            $find
        );
    }
}
