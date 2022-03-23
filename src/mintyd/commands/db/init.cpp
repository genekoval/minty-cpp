#include "commands.h"
#include "../options/opts.h"
#include "../../api/api.h"
#include "../../db/db.h"

using namespace commline;

namespace {
    auto $init(
        const app& app,
        std::string_view confpath,
        bool skip_index
    ) -> void {
        const auto settings = minty::conf::initialize(confpath);
        const auto db = minty::cli::database(settings);

        db.init();
        if (!skip_index) minty::cli::api_container(settings).api().reindex();
    }
}

namespace minty::cli {
    auto init(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "init",
            "Initialize the database",
            options(
                opts::config(confpath),
                flag({"skip-index"}, "Skip search engine index operation")
            ),
            arguments(),
            $init
        );
    }
}
