#include "repo/repo.hpp"
#include "commands/commands.h"
#include "commands/options/opts.h"

#include <internal/core/preview.hpp>
#include <internal/server/server.hpp>
#include <internal/server/http/server.hpp>

#include <http/http>
#include <dmon/dmon>
#include <filesystem>
#include <fmt/format.h>

namespace fs = std::filesystem;

using minty::server::server_list;

namespace {
    namespace internal {
        constexpr auto crash_log_level = timber::level::critical;
        constexpr auto signals = std::array { SIGINT, SIGTERM };

        const auto default_config = fs::path(CONFDIR) / "minty.yml";

        auto default_pidfile(std::string_view name) -> fs::path {
            auto path = fs::temp_directory_path() / name;
            path += ".pid";
            return path;
        }

        auto handle_signals(
            server_list& servers,
            http::server::server_list& http_servers
        ) -> ext::jtask<> {
            auto signalfd = netcore::signalfd::create(signals);

            while (true) {
                const auto signal = co_await signalfd.wait_for_signal();

                TIMBER_INFO(
                    "Received signal ({}): {}",
                    signal,
                    strsignal(signal)
                );

                servers.close();
                http_servers.close();
            }
        }

        auto main(
            const commline::app& app,
            std::string_view confpath,
            bool daemon,
            std::optional<fs::path> pidfile
        ) -> void {
            auto startup_timer = timber::timer(
                "Server started in",
                timber::level::info
            );

            const auto settings = minty::conf::initialize(confpath);

            if (daemon && !dmon::daemonize({
                .group = settings.daemon.group,
                .identifier = app.name,
                .pidfile = pidfile.value_or(default_pidfile(app.name)),
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

                auto routes = minty::server::http::router(info, repo);
                auto http = co_await minty::server::http::listen(
                    routes,
                    settings.http.cert,
                    settings.http.key,
                    settings.http.listen
                );

                auto router = minty::server::make_router(repo, info);
                auto servers = co_await minty::server::listen(
                    router,
                    settings.server
                );

                startup_timer.stop();
                auto uptime_timer = timber::timer(
                    "Server shutting down. Up",
                    timber::level::notice
                );

                const auto sigtask = handle_signals(servers, http);
                co_await servers.join();
                co_await http.join();

                uptime_timer.stop();
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
            ),
            commline::option<std::optional<fs::path>>(
                {"p", "pidfile"},
                "Path to the pidfile",
                "path"
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
