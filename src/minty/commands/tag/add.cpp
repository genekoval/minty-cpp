#include "cli.h"
#include "commands.h"

#include "../../client.h"
#include "../../output.h"
#include "../../options/opts.h"

#include <iostream>

using namespace commline;

namespace {
    auto $add(
        const app& app,
        const std::vector<std::string_view>& aliases,
        std::optional<std::string_view> description,
        const std::vector<std::string_view>& links,
        std::optional<std::string_view> path,
        std::string_view name
    ) -> void {
        auto api = minty::cli::client();
        const auto id = api.add_tag(name);

        if (description) api.set_tag_description(id, *description);

        for (const auto& alias : aliases) {
            api.add_tag_alias(id, alias);
        }

        for (const auto& link : links) {
            api.add_tag_source(id, link);
        }

        minty::cli::print_tag(api, id, path);
    }
}

namespace minty::commands {
    auto add() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Add a tag",
            options(
                cli::opts::aliases(),
                cli::opts::description(),
                cli::opts::links(),
                cli::opts::path()
            ),
            arguments(
                required<std::string_view>("name")
            ),
            $add
        );
    }
}
