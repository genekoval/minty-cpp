#include "../api/api.h"
#include "commands.h"

#include <minty/conf/settings.h>

using namespace commline;

namespace {
    auto $reindex(
        const app& app,
        const argv& argv,
        std::string_view confpath
    ) -> void {
        const auto settings = minty::conf::settings::load_file(
            std::string(confpath)
        );
        timber::reporting_level = settings.log.level;

        auto container = minty::cli::api_container(settings);

        container.api().reindex();

        std::cout << "Minty search reindex complete" << std::endl;
    }
}

namespace minty::cli {
    auto reindex(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "reindex",
            "Reindex the search engine",
            options(
                option<std::string_view>(
                    {"config", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                )
            ),
            $reindex
        );
    }
}
