#include "router/router.h"

#include <minty/net/zipline/transfer.h>
#include <minty/server/server.h>

#include <timber/timber>

namespace minty::server {
    auto listen(
        core::api& api,
        const server_info& info,
        std::string_view endpoint,
        std::function<void()>&& callback
    ) -> void {
        auto ctx = context(api, info);
        auto routes = make_router(ctx);

        auto server = netcore::server([&routes](netcore::socket&& sock) {
            auto socket = net::socket(std::move(sock));
            routes.route(socket);
        });

        server.listen(endpoint, callback);
    }
}
