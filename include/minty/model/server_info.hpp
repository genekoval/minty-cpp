#pragma once

#include <string>
#include <uuid++/uuid++>
#include <zipline/zipline>

namespace minty {
    struct server_info {
        struct object_source_type {
            std::optional<std::string> host;
            std::uint16_t port;
            UUID::uuid bucket_id;
        };

        std::string version;
        object_source_type object_source;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::server_info::object_source_type,
        &minty::server_info::object_source_type::host,
        &minty::server_info::object_source_type::port,
        &minty::server_info::object_source_type::bucket_id
    );

    ZIPLINE_OBJECT(
        minty::server_info,
        &minty::server_info::version,
        &minty::server_info::object_source
    );
}
