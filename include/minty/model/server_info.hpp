#pragma once

#include <ext/json.hpp>
#include <string>
#include <uuid++/json.hpp>
#include <uuid++/uuid++>
#include <zipline/zipline>

namespace minty {
    struct object_source_type {
        std::optional<std::string> host;
        std::uint16_t port;
        UUID::uuid bucket_id;
    };

    struct server_info {
        std::string version;
        object_source_type object_source;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        object_source_type,
        host,
        port,
        bucket_id
    );

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        server_info,
        version,
        object_source
    );
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::object_source_type,
        &minty::object_source_type::host,
        &minty::object_source_type::port,
        &minty::object_source_type::bucket_id
    );

    ZIPLINE_OBJECT(
        minty::server_info,
        &minty::server_info::version,
        &minty::server_info::object_source
    );
}
