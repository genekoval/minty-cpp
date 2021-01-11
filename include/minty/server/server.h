#pragma once

#include <minty/core/api.h>
#include <netcore/socket.h>

namespace minty::server {
    using event_t = unsigned int;

    struct protocol : zipline::protocol<netcore::socket> {
        core::api* api;

        protocol(const netcore::socket& sock, core::api& api);
    };

    auto listen(
        core::api& api,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void;
}
