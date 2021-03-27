#include "endpoints/endpoints.h"

#include <minty/server/server.h>

#include <timber/timber>

namespace minty::server {
    static inline auto router() {
        return zipline::make_router<protocol, net::event_t>(
            endpoint::add_comment,
            endpoint::add_post,
            endpoint::add_tag,
            endpoint::delete_post,
            endpoint::get_comments,
            endpoint::get_post,
            endpoint::get_server_info,
            endpoint::get_tag,
            endpoint::get_tag_posts,
            endpoint::get_tag_previews
        );
    }

    protocol::protocol(
        net::socket& sock,
        const server_info& info,
        core::api& api
    ) :
        zipline::protocol<net::socket>(sock),
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
            auto socket = net::socket(std::move(sock));
            routes.route(protocol(socket, info, api));
        });

        server.listen(endpoint, callback);

        INFO() << "Shutting down...";
    }
}
