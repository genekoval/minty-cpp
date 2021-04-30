#include "routes/routes.h"

#include <minty/net/zipline/transfer.h>
#include <minty/server/server.h>

#include <timber/timber>

#define MINTY_ROUTE(r) ZIPLINE_ROUTE(route, protocol, r)

#define MINTY_ROUTER(...) ZIPLINE_ROUTER(protocol, net::event_t, __VA_ARGS__)

namespace minty::server {
    MINTY_ROUTE(add_comment)
    MINTY_ROUTE(add_post)
    MINTY_ROUTE(add_tag)
    MINTY_ROUTE(add_tag_alias)
    MINTY_ROUTE(add_tag_source)
    MINTY_ROUTE(delete_post)
    MINTY_ROUTE(delete_tag)
    MINTY_ROUTE(delete_tag_alias)
    MINTY_ROUTE(delete_tag_source)
    MINTY_ROUTE(get_comments)
    MINTY_ROUTE(get_post)
    MINTY_ROUTE(get_server_info)
    MINTY_ROUTE(get_tag)
    MINTY_ROUTE(get_tags_by_name)
    MINTY_ROUTE(get_tag_posts)
    MINTY_ROUTE(get_tag_previews)
    MINTY_ROUTE(set_tag_description)
    MINTY_ROUTE(set_tag_name)

    MINTY_ROUTER(
        add_comment,
        add_post,
        add_tag,
        add_tag_alias,
        add_tag_source,
        delete_post,
        delete_tag,
        delete_tag_alias,
        delete_tag_source,
        get_comments,
        get_post,
        get_server_info,
        get_tag,
        get_tags_by_name,
        get_tag_posts,
        get_tag_previews,
        set_tag_description,
        set_tag_name
    )

    auto listen(
        std::string_view endpoint,
        context& ctx,
        std::function<void()>&& callback
    ) -> void {
        const auto routes = router();

        auto server = netcore::server([&ctx, &routes](netcore::socket&& sock) {
            auto socket = net::socket(std::move(sock));
            routes.route(protocol(ctx, socket));
        });

        server.listen(endpoint, callback);
    }
}
