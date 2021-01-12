#include <minty/client/api.h>

#include <netcore/client.h>
#include <zipline/transfer.h>

namespace minty {
    api::api(std::string_view endpoint) : endpoint(endpoint) {}

    auto api::connect() -> client {
        socket = netcore::connect(endpoint);
        return client(socket);
    }

    auto api::add_creator(std::string_view name) -> std::string {
        return connect().send<std::string>(
            event::add_creator,
            name
        );
    }
}
