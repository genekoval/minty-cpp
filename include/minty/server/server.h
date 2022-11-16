#pragma once

#include <minty/core/api.h>
#include <minty/server/server_info.h>

namespace minty::server {
    auto create(
        core::api& api,
        const server_info& info
    ) -> netcore::server;
}
