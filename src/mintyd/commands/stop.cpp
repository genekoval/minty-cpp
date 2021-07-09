#include "commands.h"

#include <minty/conf/settings.h>

#include <dmon/dmon>

using namespace commline;

namespace {
    auto $stop(
        const app& app,
        const argv& argv,
        std::string_view confpath
    ) -> void {
        const auto settings = minty::conf::settings::load_file(
            std::string(confpath)
        );
        timber::reporting_level = settings.log.level;

        dmon::stop(settings.daemon.pidfile);
    }
}

namespace minty::cli {
    auto stop(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "stop",
            "Stop the server daemon",
            options(
                option<std::string_view>(
                    {"config", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                )
            ),
            $stop
        );
    }
}
