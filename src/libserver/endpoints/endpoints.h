#pragma once

#include <minty/server/server.h>

namespace minty::server::endpoint {
    auto add_creator(protocol&) -> void;
    auto get_creator(protocol&) -> void;
}
