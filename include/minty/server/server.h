#pragma once

#include <minty/core/api.h>
#include <minty/server/server_info.h>

#include <netcore/socket.h>

namespace minty::server {
    using socket_t = netcore::socket;

    struct protocol : zipline::protocol<socket_t> {
        core::api* api;
        const server_info* info;

        protocol(const socket_t& sock, const server_info& info, core::api& api);
    };

    auto listen(
        std::string_view endpoint,
        const server_info& info,
        core::api& api,
        const std::function<void()>& callback
    ) -> void;
}
