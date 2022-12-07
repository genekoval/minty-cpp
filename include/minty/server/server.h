#pragma once

#include "router.hpp"

#include <minty/core/api.h>
#include <minty/net/zipline/coder.h>
#include <minty/server/server_info.h>

namespace minty::server {
    namespace detail {
        template <typename... Routes>
        auto make_router(router_context&& context, Routes&&... routes) {
            return zipline::router<
                net::socket,
                net::event_t,
                net::error_list,
                router_context,
                Routes...
            >(
                std::forward<router_context>(context),
                std::forward<Routes>(routes)...
            );
        }
    }

    inline auto make_router(
        core::api& api,
        const server_info& info
    ) {
        return detail::make_router(
            router_context(api, info),
            &router_context::add_comment,
            &router_context::add_object_data,
            &router_context::add_objects_url,
            &router_context::add_post,
            &router_context::add_post_objects,
            &router_context::add_post_tag,
            &router_context::add_related_post,
            &router_context::add_reply,
            &router_context::add_tag,
            &router_context::add_tag_alias,
            &router_context::add_tag_source,
            &router_context::delete_post,
            &router_context::delete_post_objects,
            &router_context::delete_post_objects_ranges,
            &router_context::delete_post_tag,
            &router_context::delete_related_post,
            &router_context::delete_tag,
            &router_context::delete_tag_alias,
            &router_context::delete_tag_source,
            &router_context::get_comment,
            &router_context::get_comments,
            &router_context::get_object,
            &router_context::get_post,
            &router_context::get_posts,
            &router_context::get_server_info,
            &router_context::get_tag,
            &router_context::get_tags,
            &router_context::move_post_object,
            &router_context::move_post_objects,
            &router_context::set_comment_content,
            &router_context::set_post_description,
            &router_context::set_post_title,
            &router_context::set_tag_description,
            &router_context::set_tag_name
        );
    }

    using router_type = std::invoke_result_t<
        decltype(make_router),
        core::api&,
        const server_info&
    >;

    class server_context {
        router_type router;
        timber::timer& startup_timer;
        timber::timer& uptime_timer;
    public:
        server_context(
            router_type&& router,
            timber::timer& startup_timer,
            timber::timer& uptime_timer
        );

        auto close() -> void;

        auto connection(netcore::socket&& client) -> ext::task<>;

        auto listen() -> void;
    };

    using server_type = netcore::server<server_context>;

    auto create(
        core::api& api,
        const server_info& info,
        timber::timer& startup_timer,
        timber::timer& uptime_timer
    ) -> server_type;
}
