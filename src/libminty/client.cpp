#include <minty/client/api.h>
#include <minty/error.h>

namespace minty {
    client::client(
        std::string_view endpoint,
        std::string_view object_host
    ) :
        pool(provider(parse_endpoint(endpoint))),
        object_client(object_host)
    {}

    auto client::bucket(
        fstore::object_store& object_store
    ) -> ext::task<fstore::bucket> {
        if (!bucket_id) {
            auto api = co_await connect();
            const auto info = co_await api->get_server_info();

            bucket_id = info.object_source.bucket_id;
        }

        co_return fstore::bucket(object_store, bucket_id);
    }

    auto client::connect() -> ext::task<ext::pool_item<api>> {
        co_return co_await pool.checkout();
    }

    auto client::object_store() ->
        ext::task<ext::pool_item<fstore::object_store>>
    {
        co_return co_await object_client.connect();
    }

    auto client::parse_endpoint(std::string_view endpoint) -> socket_type {
        if (endpoint.starts_with("/")) {
            return domain { .path = std::string(endpoint) };
        }

        // The endpoint is a host and port separated by a colon.
        const auto delim = endpoint.find(":");

        if (delim == std::string_view::npos) {
            throw minty::minty_error(
                "port missing from endpoint: {}",
                endpoint
            );
        }

        return network {
            .host = std::string(endpoint.substr(0, delim)),
            .port = std::string(endpoint.substr(delim + 1))
        };
    }

    client::provider::provider() : endpoint(std::monostate{}) {}

    client::provider::provider(socket_type&& endpoint) :
        endpoint(std::forward<socket_type>(endpoint))
    {}

    auto client::provider::provide() -> ext::task<api> {
        auto socket = netcore::socket();

        if (const auto* sock = std::get_if<domain>(&endpoint)) {
            socket = co_await netcore::connect(sock->path);
        }
        else if (const auto* sock = std::get_if<network>(&endpoint)) {
            socket = co_await netcore::connect(
                sock->host,
                sock->port
            );
        }

        co_return api(std::move(socket));
    }
}
