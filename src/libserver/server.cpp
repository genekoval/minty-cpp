#include "context/context.h"

#include <minty/net/zipline/transfer.h>
#include <minty/net/zipline/protocol.h>
#include <minty/server/server.h>

using minty::server::context;

template <typename ...Routes>
using router = zipline::router<
    minty::net::socket,
    minty::net::event_t,
    minty::net::error_list,
    context,
    Routes...
>;

namespace {
    auto make_router(context& ctx) {
        return router(
            ctx,
            &context::add_comment,
            &context::add_object_data,
            &context::add_object_local,
            &context::add_objects_url,
            &context::add_post,
            &context::add_post_objects,
            &context::add_post_tag,
            &context::add_tag,
            &context::add_tag_alias,
            &context::add_tag_source,
            &context::delete_post,
            &context::delete_post_objects,
            &context::delete_post_objects_ranges,
            &context::delete_post_tag,
            &context::delete_tag,
            &context::delete_tag_alias,
            &context::delete_tag_source,
            &context::get_comments,
            &context::get_object,
            &context::get_post,
            &context::get_posts,
            &context::get_server_info,
            &context::get_tag,
            &context::get_tags,
            &context::move_post_object,
            &context::move_post_objects,
            &context::set_comment_content,
            &context::set_post_description,
            &context::set_post_title,
            &context::set_tag_description,
            &context::set_tag_name
        );
    }
}

namespace minty::server {
    auto listen(
        core::api& api,
        const server_info& info,
        const netcore::unix_socket& unix_socket,
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
