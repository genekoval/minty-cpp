#include "commands.h"

#include "../../client.h"
#include "../../options/opts.h"
#include "../../output.h"

using namespace commline;

namespace {
    auto $find(
        const commline::app& app,
        int from,
        int size,
        std::optional<std::string_view> path,
        const std::string& name
    ) -> void {
        const auto query = minty::core::tag_query {
            .from = static_cast<unsigned int>(from),
            .size = static_cast<unsigned int>(size),
            .name = name
        };

        auto api = minty::cli::client();
        minty::cli::print(api.get_tags(query), path);
    }
}

namespace minty::commands {
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
