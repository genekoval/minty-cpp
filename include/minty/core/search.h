#pragma once

#include <minty/net/zipline/protocol.h>

#include <netcore/netcore>
#include <zipline/zipline>

namespace minty::core {
    class search_engine {
        enum class event : net::event_t {
            find_tags_by_name,
            version
        };

        using protocol = zipline::protocol<net::socket>;
        using client = zipline::client<protocol, event>;

        const std::string endpoint;

        auto connect() -> client;

        auto version() -> std::string;
    public:
        search_engine(std::string_view endpoint);

        auto find_tags_by_name(
            std::string_view term
        ) -> std::vector<std::string>;
    };
}
