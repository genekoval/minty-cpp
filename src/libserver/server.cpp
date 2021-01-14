#include "endpoints/endpoints.h"

#include <minty/net/zipline/protocol.h>
#include <minty/server/server.h>

#include <netcore/server.h>
#include <timber/timber>

namespace minty::server {
    static inline auto router() {
        return zipline::make_router<protocol, net::event_t>(
            endpoint::add_creator,
            endpoint::get_creator
        );
    }

    protocol::protocol(const netcore::socket& sock, core::api& api) :
        zipline::protocol<netcore::socket>(sock),
        api(&api)
    {}

    auto listen(
        core::api& api,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void {
        const auto routes = router();

        auto server = netcore::server(
            [&api, &routes](netcore::socket&& sock) {
                routes.route(protocol(sock, api));
            }
        );

        server.listen(endpoint, callback);

        INFO() << "Shutting down...";
    }
}
