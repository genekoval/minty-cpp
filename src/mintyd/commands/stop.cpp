#include "commands.h"
#include "options/opts.h"

#include <minty/conf/settings.h>

#include <dmon/dmon>

using namespace commline;

namespace {
    auto $stop(
        const app& app,
        std::string_view confpath
    ) -> void {
        const auto settings = minty::conf::initialize(confpath);

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
                opts::config(confpath)
            ),
            arguments(),
            $stop
        );
    }
}
