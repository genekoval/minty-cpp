#pragma once

#include <minty/server/server_info.h>

#include <zipline/zipline>

namespace zipline {
    ZIPLINE_OBJECT(
        minty::server::server_info::object_source_type,
        &minty::server::server_info::object_source_type::host,
        &minty::server::server_info::object_source_type::port,
        &minty::server::server_info::object_source_type::bucket_id
    );

    ZIPLINE_OBJECT(
        minty::server::server_info,
        &minty::server::server_info::version,
        &minty::server::server_info::object_source
    );
}
