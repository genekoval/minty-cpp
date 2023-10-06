#pragma once

#include <cstdint>

namespace minty {
    enum class event : std::uint32_t {
        add_comment,
        add_object_data,
        add_objects_url,
        add_post_objects,
        add_post_tag,
        add_related_post,
        add_reply,
        add_tag,
        add_tag_alias,
        add_tag_source,
        create_post,
        create_post_draft,
        delete_comment_tree,
        delete_post,
        delete_post_objects,
        delete_post_tag,
        delete_related_post,
        delete_tag,
        delete_tag_alias,
        delete_tag_source,
        get_comment,
        get_comments,
        get_object,
        get_post,
        get_posts,
        get_server_info,
        get_tag,
        get_tags,
        move_post_objects,
        set_comment_content,
        set_post_description,
        set_post_title,
        set_tag_description,
        set_tag_name
    };
}
