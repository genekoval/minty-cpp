#include "commands.h"
#include "../options/opts.h"
#include "../../db/db.h"

using namespace commline;

namespace {
    auto $migrate(
        const app& app,
        std::string_view confpath
    ) -> void {
        const auto settings = minty::conf::initialize(confpath);
        const auto client = minty::cli::data::client(settings);

        client.migrate();
    }
}

namespace minty::cli {
    auto migrate(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Update schemas to the current program version",
            options(
                opts::config(confpath)
            ),
            arguments(),
            $migrate
        );
    }
}
