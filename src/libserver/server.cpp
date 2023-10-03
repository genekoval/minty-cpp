#include <internal/server/server.hpp>

#include <ext/data_size.h>

using namespace ext::literals;

using conftools::endpoint;
using netcore::address_type;

namespace {
    auto on_error(const endpoint& endpoint, std::exception_ptr exception)
        -> void {
        try {
            if (exception) std::rethrow_exception(exception);
        }
        catch (const std::exception& ex) {
            TIMBER_ERROR(ex.what());
        }
    }
}

namespace minty::server {
    server_context::server_context(router_type& router, seconds timeout) :
        router(&router),
        timeout(timeout) {}

    auto server_context::close() -> void {}

    auto server_context::connection(netcore::socket&& client) -> ext::task<> {
        auto socket =
            minty::socket(std::forward<netcore::socket>(client), 8_KiB);
        co_await router->route(socket, timeout);
    }

    auto server_context::listen(const address_type& address) -> void {
        TIMBER_INFO("Listening for connections on {}", address);
    }

    auto listen(router_type& router, std::span<const endpoint> endpoints)
        -> ext::task<server_list> {
        using endpoint_ref =
            std::optional<std::reference_wrapper<const netcore::endpoint>>;

        co_return co_await server_list::listen(
            endpoints,
            [&router](const endpoint& endpoint, endpoint_ref& out) {
                out.emplace(endpoint.endpoint);
                return server(router, endpoint.timeout);
            },
            on_error
        );
    }
}
