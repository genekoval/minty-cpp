#include "../api/api.h"
#include "commands.h"
#include "options/opts.h"

using namespace commline;

namespace {
    auto $reindex(
        const app& app,
        std::string_view confpath
    ) -> void {
        const auto settings = minty::conf::initialize(confpath);
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
                opts::config(confpath)
            ),
            arguments(),
            $reindex
        );
    }
}
