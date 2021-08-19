#include "../api/api.h"
#include "commands.h"

#include <minty/conf/settings.h>

#include <fmt/format.h>

using namespace commline;

namespace {
    auto $prune(
        const app& app,
        const argv& argv,
        std::string_view confpath
    ) -> void {
        const auto settings = minty::conf::initialize(confpath);

        auto container = minty::cli::api_container(settings);
        container.api().prune();
    }
}

namespace minty::cli {
    auto prune(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "prune",
            "Delete unused data",
            options(
                option<std::string_view>(
                    {"config", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                )
            ),
            $prune
        );
    }
}
