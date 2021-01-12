#include "client.h"
#include "commands/commands.h"

#include <minty/minty>
#include <minty/core/settings.h>

#include <iostream>
#include <filesystem>
#include <timber/timber>

const auto default_config = std::filesystem::path(CONFDIR) / "minty.yml";

namespace minty::cli {
    auto client() -> api {
        const auto settings = minty::core::settings::load_file(default_config);
        return api(settings.connection);
    }
}

static auto $main(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    std::cout << app.name << ": " << app.description << std::endl;
}

auto main(int argc, const char** argv) -> int {
    timber::reporting_level() = timber::level::info;

    auto app = commline::application(
        NAME,
        VERSION,
        DESCRIPTION,
        $main
    );

    app.subcommand(minty::commands::creator());

    return app.run(argc, argv);
}
