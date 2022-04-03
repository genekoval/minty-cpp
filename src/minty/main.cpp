#include "client.h"
#include "commands/commands.h"

#include <minty/minty>
#include <minty/conf/settings.h>

#include <iostream>
#include <filesystem>
#include <timber/timber>

namespace fs = std::filesystem;

namespace {
    const auto config_directory = fs::path(CONFDIR);
    const auto default_config = (config_directory / "minty.yml").string();

    auto settings() -> const minty::conf::settings& {
        static const auto instance = minty::conf::initialize(default_config);
        return instance;
    }

    auto $main(const commline::app& app, bool version) -> void {
        if (!version) {
            std::cout << app.name << ": " << app.description << std::endl;
            return;
        }

        commline::print_version(std::cout, app);

        auto api = minty::cli::client();
        const auto server_info = api.get_server_info();
        std::cout << "server version: " << server_info.version << std::endl;
    }
}

namespace minty::cli {
    auto bucket() -> fstore::bucket {
        static auto object_store = fstore::object_store(
            settings().fstore.connection
        );

        const auto bkt = object_store.fetch_bucket(settings().fstore.bucket);
        return fstore::bucket(object_store, bkt.id);
    }

    auto client() -> api {
        return api(settings().server.path.string());
    }
}

auto main(int argc, const char** argv) -> int {
    using namespace commline;

    timber::reporting_level = timber::level::info;
    timber::log_handler = &timber::console_logger;

    auto app = application(
        NAME,
        VERSION,
        DESCRIPTION,
        options(
            flag(
                {"v", "version"},
                "Print version information"
            )
        ),
        arguments(),
        $main
    );

    app.subcommand(minty::commands::object());
    app.subcommand(minty::commands::post());
    app.subcommand(minty::commands::reply());
    app.subcommand(minty::commands::tag());

    return app.run(argc, argv);
}
