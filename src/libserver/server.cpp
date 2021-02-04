#include "endpoints/endpoints.h"

#include <minty/net/zipline/protocol.h>
#include <minty/server/server.h>

#include <netcore/server.h>
#include <timber/timber>

namespace minty::server {
    static inline auto router() {
        return zipline::make_router<protocol, net::event_t>(
            endpoint::add_creator,
            endpoint::add_post,
            endpoint::get_creator,
            endpoint::get_creator_posts,
            endpoint::get_creator_previews,
            endpoint::get_post,
            endpoint::get_server_info
        );
    }

    protocol::protocol(
        const socket_t& sock,
        const server_info& info,
        core::api& api
    ) :
        zipline::protocol<socket_t>(sock),
        api(&api),
        info(&info)
    {}

    auto listen(
        std::string_view endpoint,
        const server_info& info,
        core::api& api,
        const std::function<void()>& callback
    ) -> void {
        const auto routes = router();

        auto server = netcore::server([&api, &info, &routes](
            netcore::socket&& sock
        ) {
            routes.route(protocol(sock, info, api));
        });

        server.listen(endpoint, callback);

        INFO() << "Shutting down...";
    }
}
