#include "api/api.h"
#include "commands/commands.h"

#include <minty/core/api.h>
#include <minty/core/downloader.h>
#include <minty/core/search.h>
#include <minty/conf/settings.h>
#include <minty/server/server.h>
#include <minty/server/server_info.h>

#include <commline/commline>
#include <dmon/dmon>
#include <ext/data_size.h>
#include <filesystem>
#include <fmt/format.h>
#include <fstore/client.h>
#include <timber/timber>

namespace fs = std::filesystem;

namespace {
    const auto default_config = fs::path(CONFDIR) / "minty.yml";

    auto build_server_info(
        const minty::conf::settings& settings,
        std::string_view version,
        std::string_view bucket_id
    ) -> minty::server::server_info {
        const auto host = [&]() -> std::string {
            if (const auto& proxy = settings.fstore.proxy) {
                return fmt::format(
                    "{}:{}",
                    proxy->host.value_or("0"),
                    proxy->port
                );
            }
            else return settings.fstore.connection;
        }();

        const auto path = fmt::format(
            "{}/{}",
            static_cast<std::underlying_type_t<fstore::event>>(
                fstore::event::get_object
            ),
            bucket_id
        );

        return {
            .object_source = fmt::format("zipline://{}/{}", host, path),
            .version = std::string(version)
        };
    }

    auto $main(
        const commline::app& app,
        const commline::argv& argv,
        bool daemon
    ) -> void {
        const auto settings = minty::conf::settings::load_file(default_config);
        timber::reporting_level = settings.log.level;

        if (daemon && !dmon::daemonize({
            .group = settings.daemon.group,
            .identifier = app.name,
            .pidfile = settings.daemon.pidfile,
            .user = settings.daemon.user
        })) return;

        NOTICE() << app.name << " version " << app.version << " starting up";

        auto container = minty::cli::api_container(settings);
        auto info = build_server_info(
            settings,
            app.version,
            container.bucket_id()
        );

        INFO() << "Object source: " << info.object_source;

        minty::server::listen(container.api(), info, settings.server, []() {
            INFO() << "Server started. Listening for connections...";
        });

        NOTICE() << app.name << " shutting down";
    }
}

auto main(int argc, const char** argv) -> int {
    timber::log_handler = &timber::console_logger;

    auto app = commline::application(
        NAME,
        VERSION,
        DESCRIPTION,
        commline::options(
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

    const auto confpath = default_config.string();

    app.subcommand(minty::cli::reindex(confpath));
    app.subcommand(minty::cli::stop(confpath));

    return app.run(argc, argv);
}
