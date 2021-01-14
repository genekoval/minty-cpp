#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

#include <netcore/socket.h>
#include <zipline/zipline>

namespace minty {
    class api {
        enum class event : net::event_t {
            add_creator,
            get_creator
        };

        using protocol = zipline::protocol<netcore::socket>;
        using client = zipline::client<protocol, event>;

        const std::string endpoint;
        netcore::socket socket;

        auto connect() -> client;
    public:
        api(std::string_view endpoint);

        auto add_creator(std::string_view name) -> std::string;

        auto get_creator(std::string_view id) -> core::creator;
    };
}
