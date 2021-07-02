#include "router/router.h"

#include <minty/net/zipline/transfer.h>
#include <minty/server/server.h>

#include <timber/timber>

namespace minty::server {
    auto listen(
        core::api& api,
        const server_info& info,
        netcore::unix_socket unix_socket,
        std::function<void()>&& callback
    ) -> void {
        auto ctx = context(api, info);
        auto routes = make_router(ctx);

        auto server = netcore::server([&routes](netcore::socket&& sock) {
            auto socket = net::socket(std::move(sock));
            routes.route(socket);
        });

        server.listen(unix_socket, callback);
    }
}
