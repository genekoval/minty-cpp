#pragma once

#include <minty/core/api.h>
#include <minty/net/zipline/protocol.h>
#include <minty/server/server_info.h>

#include <zipline/zipline>

namespace minty::server {
    struct context {
        core::api* api;
        const server_info* info;
    };

    using protocol = zipline::server_protocol<context, net::socket>;

    auto listen(
        std::string_view endpoint,
        context& ctx,
        std::function<void()>&& callback
    ) -> void;
}
