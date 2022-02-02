#include "api/api.h"
#include "commands/commands.h"

#include <minty/conf/settings.h>
#include <minty/server/server.h>
#include <minty/server/server_info.h>

#include <dmon/dmon>
#include <filesystem>
#include <fmt/format.h>

namespace fs = std::filesystem;

namespace {
    const auto default_config = fs::path(CONFDIR) / "minty.yml";

    auto $main(
        const commline::app& app,
        const commline::argv& argv,
        std::string_view confpath,
        bool daemon
    ) -> void {
        const auto settings = minty::conf::initialize(confpath);

        if (daemon && !dmon::daemonize({
            .group = settings.daemon.group,
            .identifier = app.name,
            .pidfile = settings.daemon.pidfile,
            .user = settings.daemon.user
        })) return;

        NOTICE() << app.name << " version " << app.version << " starting up";

        auto container = minty::cli::api_container(settings);
        auto info = minty::server::server_info {
            .version = std::string(app.version),
            .object_source = {
                .host = settings.fstore.proxy.host,
                .port = settings.fstore.proxy.port,
                .bucket_id = std::string(container.bucket_id())
            }
        };

        minty::server::listen(container.api(), info, settings.server, []() {
            INFO() << "Server started. Listening for connections...";
        });

        NOTICE() << app.name << " shutting down";
    }
}

auto main(int argc, const char** argv) -> int {
    timber::log_handler = &timber::console_logger;

    const auto confpath = default_config.string();

    auto app = commline::application(
        NAME,
        VERSION,
        DESCRIPTION,
        commline::options(
            commline::option<std::string_view>(
                {"config", "c"},
                "Path to configuration file",
                "path",
                std::string_view(confpath)
            ),
            commline::flag(
                {"daemon", "d"},
                "Run the program as a daemon."
            )
        ),
        $main
    );

    app.on_error([](const auto& e) -> void {
        CRITICAL() << e.what();
    });

    app.subcommand(minty::cli::db(confpath));
    app.subcommand(minty::cli::dump(confpath));
    app.subcommand(minty::cli::prune(confpath));
    app.subcommand(minty::cli::regen(confpath));
    app.subcommand(minty::cli::reindex(confpath));
    app.subcommand(minty::cli::restore(confpath));
    app.subcommand(minty::cli::stop(confpath));

    return app.run(argc, argv);
}
