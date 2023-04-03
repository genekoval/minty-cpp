#include <internal/server/server.hpp>

namespace minty::server {
    server_context::server_context(
        router_type&& router,
        timber::timer& startup_timer,
        timber::timer& uptime_timer
    ) :
        router(std::forward<router_type>(router)),
        startup_timer(startup_timer),
        uptime_timer(uptime_timer)
    {}

    auto server_context::close() -> void {
        uptime_timer.stop();
    }

    auto server_context::connection(netcore::socket&& client) -> ext::task<> {
        auto socket = minty::socket(std::forward<netcore::socket>(client));
        co_await router.route(socket);
    }

    auto server_context::listen() -> void {
        startup_timer.stop();
        uptime_timer.reset();
    }

    auto create(
        core::repo& repo,
        const server_info& info,
        timber::timer& startup_timer,
        timber::timer& uptime_timer
    ) -> server_type {
        return server_type(
            make_router(repo, info),
            startup_timer,
            uptime_timer
        );
    }
}
