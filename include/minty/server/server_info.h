#pragma once

#include <string>

namespace minty::server {
    struct server_info {
        struct object_source_type {
            std::optional<std::string> host;
            unsigned short port;
            std::string bucket_id;
        };

        std::string version;
        object_source_type object_source;
    };
}
