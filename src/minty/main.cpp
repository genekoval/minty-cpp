#include "client.h"
#include "commands/commands.h"

#include <minty/minty>
#include <minty/conf/settings.h>

#include <iostream>
#include <filesystem>
#include <timber/timber>

const auto default_config = std::filesystem::path(CONFDIR) / "minty.yml";

namespace minty::cli {
    auto client() -> api {
        const auto settings = minty::conf::settings::load_file(default_config);
        return api(settings.connection);
    }
}

static auto $main(
    const commline::app& app,
    const commline::argv& argv,
    bool version
) -> void {
    if (!version) {
        std::cout << app.name << ": " << app.description << std::endl;
        return;
    }

    commline::print_version(std::cout, app);

    auto api = minty::cli::client();
    const auto server_info = api.get_server_info();
    std::cout << "server version: " << server_info.version << std::endl;
}

auto main(int argc, const char** argv) -> int {
    using namespace commline;

    timber::reporting_level() = timber::level::info;

    auto app = application(
        NAME,
        VERSION,
        DESCRIPTION,
        options(
            flag(
                {"version", "v"},
                "Print version information."
            )
        ),
        $main
    );

    app.subcommand(minty::commands::creator());
    app.subcommand(minty::commands::post());

    return app.run(argc, argv);
}
