#include "api/api.h"
#include "commands/commands.h"
#include "commands/options/opts.h"

#include <minty/conf/settings.h>
#include <minty/core/preview.h>
#include <minty/server/server.h>
#include <minty/server/server_info.h>

#include <http/http>
#include <dmon/dmon>
#include <filesystem>
#include <fmt/format.h>

namespace fs = std::filesystem;

namespace {
    namespace internal {
        const auto default_config = fs::path(CONFDIR) / "minty.yml";

        auto main(
            const commline::app& app,
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

            auto startup_timer = timber::timer(
                "Server started in",
                timber::level::info
            );

            auto uptime_timer = timber::timer(
                "Server shutting down. Up",
                timber::level::notice
            );

            TIMBER_NOTICE("{} version {} starting up", app.name, app.version);

            minty::cli::api(settings, [
                &settings,
                &startup_timer,
                &uptime_timer,
                version = app.version
            ](auto& api) -> ext::task<> {
                const auto info = minty::server::server_info {
                    .version = std::string(version),
                    .object_source = {
                        .host = settings.fstore.proxy.host,
                        .port = settings.fstore.proxy.port,
                        .bucket_id = api.get_bucket_id()
                    }
                };

                auto server = minty::server::create(
                    api,
                    info
                );

                co_await server.listen(settings.server, [
                    &startup_timer,
                    &uptime_timer
                ]() {
                    startup_timer.stop();
                    uptime_timer.reset();
                });
            });

            uptime_timer.stop();
        }
    }
}

auto main(int argc, const char** argv) -> int {
    std::locale::global(std::locale(""));

    timber::thread_name = "main";
    timber::log_handler = &timber::console_logger;

    minty::core::initialize_image_previews();
    const auto init = http::init();

    const auto confpath = internal::default_config.string();

    auto app = commline::application(
        NAME,
        VERSION,
        DESCRIPTION,
        commline::options(
            minty::cli::opts::config(confpath),
            commline::flag(
                {"d", "daemon"},
                "Run the program as a daemon."
            )
        ),
        commline::arguments(),
        internal::main
    );

    app.on_error([](const auto& e) -> void {
        TIMBER_CRITICAL(e.what());
    });

    app.subcommand(minty::cli::db(confpath));
    app.subcommand(minty::cli::dump(confpath));
    app.subcommand(minty::cli::errors(confpath));
    app.subcommand(minty::cli::init(confpath));
    app.subcommand(minty::cli::migrate(confpath));
    app.subcommand(minty::cli::prune(confpath));
    app.subcommand(minty::cli::regen(confpath));
    app.subcommand(minty::cli::reindex(confpath));
    app.subcommand(minty::cli::restore(confpath));

    return app.run(argc, argv);
}
