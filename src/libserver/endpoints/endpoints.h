#pragma once

#include <minty/server/server.h>

namespace minty::server::endpoint {
    auto add_comment(protocol&) -> void;
    auto add_post(protocol&) -> void;
    auto add_tag(protocol&) -> void;
    auto delete_post(protocol&) -> void;
    auto get_comments(protocol&) -> void;
    auto get_post(protocol&) -> void;
    auto get_server_info(protocol&) -> void;
    auto get_tag(protocol&) -> void;
    auto get_tags_by_name(protocol&) -> void;
    auto get_tag_posts(protocol&) -> void;
    auto get_tag_previews(protocol&) -> void;
}
