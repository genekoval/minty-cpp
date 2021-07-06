#include "client.h"
#include "commands/commands.h"

#include <minty/minty>
#include <minty/conf/settings.h>

#include <iostream>
#include <filesystem>
#include <timber/timber>

namespace fs = std::filesystem;

namespace {
    const auto default_config = fs::path(CONFDIR) / "minty.yml";

    auto $main(
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

    auto console_logger(const timber::log& l) noexcept -> void {
        if (l.log_level != timber::level::info) {
            std::cerr << l.log_level << ": ";
        }

        std::cerr << l.stream.str() << std::endl;
    }
}

namespace minty::cli {
    auto client() -> api {
        const auto settings = minty::conf::settings::load_file(default_config);
        return api(settings.server.path.string());
    }
}

auto main(int argc, const char** argv) -> int {
    using namespace commline;

    timber::reporting_level = timber::level::info;
    timber::log_handler = &console_logger;

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

    app.subcommand(minty::commands::post());
    app.subcommand(minty::commands::tag());

    return app.run(argc, argv);
}
