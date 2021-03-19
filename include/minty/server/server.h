#pragma once

#include <minty/core/api.h>
#include <minty/net/zipline/protocol.h>
#include <minty/server/server_info.h>

namespace minty::server {
    struct protocol : zipline::protocol<net::socket> {
        core::api* api;
        const server_info* info;

        protocol(net::socket& sock, const server_info& info, core::api& api);
    };

    auto listen(
        std::string_view endpoint,
        const server_info& info,
        core::api& api,
        const std::function<void()>& callback
    ) -> void;
}
