#include "router.h"

#include <minty/net/zipline/transfer.h>

namespace minty::server {
    auto make_router(context& ctx) -> router {
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
            &context::delete_post_tag,
            &context::delete_tag,
            &context::delete_tag_alias,
            &context::delete_tag_source,
            &context::get_comments,
            &context::get_object,
            &context::get_post,
            &context::get_server_info,
            &context::get_tag,
            &context::get_tag_posts,
            &context::get_tag_previews,
            &context::get_tags_by_name,
            &context::move_post_object,
            &context::set_post_description,
            &context::set_post_title,
            &context::set_tag_description,
            &context::set_tag_name
        );
    }
}
