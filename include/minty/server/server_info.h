#pragma once

#include <string>

namespace minty::server {
    struct server_info {
        const std::string object_source;
        const std::string version;
    };
}
