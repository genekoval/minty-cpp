#pragma once

#include "../context/context.h"

namespace minty::server {
    using router = zipline::router<
        net::socket,
        net::event_t,
        net::error_list,
        context,
        decltype(&context::add_comment),
        decltype(&context::add_post),
        decltype(&context::add_tag),
        decltype(&context::add_tag_alias),
        decltype(&context::add_tag_source),
        decltype(&context::delete_post),
        decltype(&context::delete_tag),
        decltype(&context::delete_tag_alias),
        decltype(&context::delete_tag_source),
        decltype(&context::get_comments),
        decltype(&context::get_object),
        decltype(&context::get_post),
        decltype(&context::get_server_info),
        decltype(&context::get_tag),
        decltype(&context::get_tag_posts),
        decltype(&context::get_tag_previews),
        decltype(&context::get_tags_by_name),
        decltype(&context::set_post_description),
        decltype(&context::set_post_title),
        decltype(&context::set_tag_description),
        decltype(&context::set_tag_name)
    >;

    auto make_router(context& ctx) -> router;
}
