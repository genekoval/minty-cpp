#pragma once

#include <minty/core/api.h>
#include <minty/server/server_info.h>

namespace minty::server {
    auto listen(
        core::api& api,
        const server_info& info,
        const netcore::unix_socket& unix_socket,
        std::function<void()>&& callback
    ) -> void;
}
