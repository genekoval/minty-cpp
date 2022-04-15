#pragma once

#include <string>
#include <uuid++/uuid++>

namespace minty::server {
    struct server_info {
        struct object_source_type {
            std::optional<std::string> host;
            unsigned short port;
            UUID::uuid bucket_id;
        };

        std::string version;
        object_source_type object_source;
    };
}
