#include "repo/repo.hpp"
#include "commands/commands.h"
#include "commands/options/opts.h"

#include <internal/core/preview.hpp>
#include <internal/server/server.hpp>

#include <http/http>
#include <dmon/dmon>
#include <filesystem>
#include <fmt/format.h>

namespace fs = std::filesystem;

namespace {
    namespace internal {
        constexpr auto crash_log_level = timber::level::critical;
        const auto default_config = fs::path(CONFDIR) / "minty.yml";

        auto main(
            const commline::app& app,
            std::string_view confpath,
            bool daemon
        ) -> void {
            auto startup_timer = timber::timer(
                "Server started in",
                timber::level::info
            );

            auto uptime_timer = timber::timer(
                "Server shutting down. Up",
                timber::level::notice
            );

            const auto settings = minty::conf::initialize(confpath);

            if (daemon && !dmon::daemonize({
                .group = settings.daemon.group,
                .identifier = app.name,
                .pidfile = settings.daemon.pidfile,
                .user = settings.daemon.user
            })) return;

            TIMBER_NOTICE("{} version {} starting up", app.name, app.version);

            minty::cli::repo(settings, [&](
                minty::core::repo& repo
            ) -> ext::task<> {
                const auto info = minty::server_info {
                    .version = std::string(app.version),
                    .object_source = {
                        .host = settings.fstore.proxy.host,
                        .port = settings.fstore.proxy.port,
                        .bucket_id = repo.get_bucket_id()
                    }
                };

                auto server = minty::server::create(
                    repo,
                    info,
                    startup_timer,
                    uptime_timer
                );

                co_await server.listen(settings.server);
            });
        }

        auto handle_error(std::exception_ptr eptr) -> void {
            try {
                if (eptr) std::rethrow_exception(eptr);
            }
            catch (const std::exception& ex) {
                TIMBER_LOG(internal::crash_log_level, ex.what());
            }
        }
    }
}

auto main(int argc, char** argv) -> int {
    std::locale::global(std::locale(""));

    timber::thread_name = "main";
    timber::log_handler = timber::console::logger;
    timber::set_terminate(internal::crash_log_level);

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

    app.on_error(internal::handle_error);

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
