#pragma once

#include <minty/server/server.h>

namespace minty::server::endpoint {
    auto add_creator(protocol&) -> void;
    auto add_post(protocol&) -> void;
    auto get_creator(protocol&) -> void;
    auto get_creator_posts(protocol&) -> void;
    auto get_creator_previews(protocol&) -> void;
    auto get_post(protocol&) -> void;
    auto get_server_info(protocol&) -> void;
}
