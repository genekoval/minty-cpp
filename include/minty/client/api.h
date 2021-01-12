#pragma once

#include <minty/net/protocol.h>

#include <netcore/socket.h>
#include <zipline/zipline>

namespace minty {
    class api {
        enum class event : net::event_t {
            add_creator
        };

        using protocol = zipline::protocol<netcore::socket>;
        using client = zipline::client<protocol, event>;

        const std::string endpoint;
        netcore::socket socket;

        auto connect() -> client;
    public:
        api(std::string_view endpoint);

        auto add_creator(std::string_view name) -> std::string;
    };
}
