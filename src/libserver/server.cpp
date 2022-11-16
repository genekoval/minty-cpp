#include "context/context.h"

#include <minty/net/zipline/coder.h>
#include <minty/net/zipline/protocol.h>
#include <minty/server/server.h>

using minty::server::context;

namespace {
    template <typename ...Routes>
    auto make_router(context&& ctx, Routes&&... routes) {
        return zipline::router<
            minty::net::socket,
            minty::net::event_t,
            minty::net::error_list,
            context,
            Routes...
        >(
            std::move(ctx),
            routes...
        );
    }

    auto make_router(
        minty::core::api& api,
        const minty::server::server_info& info
    ) {
        return make_router(
            context(api, info),
            &context::add_comment,
            &context::add_object_data,
            &context::add_objects_url,
            &context::add_post,
            &context::add_post_objects,
            &context::add_post_tag,
            &context::add_related_post,
            &context::add_reply,
            &context::add_tag,
            &context::add_tag_alias,
            &context::add_tag_source,
            &context::delete_post,
            &context::delete_post_objects,
            &context::delete_post_objects_ranges,
            &context::delete_post_tag,
            &context::delete_related_post,
            &context::delete_tag,
            &context::delete_tag_alias,
            &context::delete_tag_source,
            &context::get_comment,
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
    auto create(
        core::api& api,
        const server_info& info
    ) -> netcore::server {
        return netcore::server([
            routes = make_router(api, info)
        ](netcore::socket&& socket) mutable -> ext::task<> {
            auto client = net::socket(std::forward<netcore::socket>(socket));
            co_await routes.route(client);
        });
    }
}
