#pragma once

#include "api.hpp"

#include <ext/pool>
#include <netcore/netcore>
#include <zipline/zipline>

namespace minty {
    class client {
        struct domain {
            std::string path;
        };

        struct network {
            std::string host;
            std::string port;
        };

        using socket_type = std::variant<std::monostate, domain, network>;

        static auto parse_endpoint(std::string_view endpoint) -> socket_type;

        class provider {
            socket_type endpoint;
        public:
            provider();

            provider(socket_type&& endpoint);

            auto provide() -> ext::task<api>;
        };

        ext::async_pool<api, provider> pool;
    public:
        client(std::string_view endpoint);

        auto connect() -> ext::task<ext::pool_item<api>>;
    };
}
