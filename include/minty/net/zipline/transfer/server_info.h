#pragma once

#include <minty/server/server_info.h>

#include <zipline/zipline>

namespace zipline {
    ZIPLINE_OBJECT(
        minty::server::server_info,
        &minty::server::server_info::object_source,
        &minty::server::server_info::version
    );
}
