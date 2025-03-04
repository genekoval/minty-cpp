#pragma once

#include "router.hpp"

#include <conftools/endpoint.hpp>
#include <minty/except.hpp>

namespace minty::server {
    namespace detail {
        template <typename... Routes>
        auto make_router(router_context&& context, Routes&&... routes) {
            return zipline::router<
                socket,
                std::underlying_type_t<event>,
                router_context,
                Routes...>(
                std::forward<router_context>(context),
                error_list::codes(),
                std::forward<Routes>(routes)...
            );
        }
    }

    inline auto make_router(core::repo& repo, const server_info& info) {
        return detail::make_router(
            router_context(repo, info),
            &router_context::add_comment,
            &router_context::add_object_data,
            &router_context::add_objects_url,
            &router_context::add_post_objects,
            &router_context::add_post_tag,
            &router_context::add_related_post,
            &router_context::add_reply,
            &router_context::add_tag,
            &router_context::add_tag_alias,
            &router_context::add_tag_source,
            &router_context::create_post,
            &router_context::create_post_draft,
            &router_context::delete_comment,
            &router_context::delete_post,
            &router_context::delete_post_objects,
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
            &router_context::set_comment_content,
            &router_context::set_post_description,
            &router_context::set_post_title,
            &router_context::set_tag_description,
            &router_context::set_tag_name
        );
    }

    using router_type = std::
        invoke_result_t<decltype(make_router), core::repo&, const server_info&>;

    class server_context {
        router_type* router;
        seconds timeout;
    public:
        server_context(router_type& router, seconds timeout);

        auto close() -> void;

        auto connection(netcore::socket&& client) -> ext::task<>;

        auto listen(const netcore::address_type& address) -> void;
    };

    using server = netcore::server<server_context>;
    using server_list = netcore::server_list<server_context>;

    auto listen(
        router_type& router,
        std::span<const conftools::endpoint> endpoints
    ) -> ext::task<server_list>;
}
