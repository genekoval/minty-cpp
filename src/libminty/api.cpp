#include <minty/client/api.h>
#include <minty/net/zipline/transfer.h>

#include <netcore/client.h>
#include <zipline/transfer.h>

namespace minty {
    api::api(std::string_view endpoint) : endpoint(endpoint) {}

    auto api::connect() -> client {
        return client(net::socket(netcore::connect(endpoint)));
    }

    auto api::add_creator(std::string_view name) -> std::string {
        return connect().send<std::string>(
            event::add_creator,
            name
        );
    }

    auto api::add_post(
        std::optional<std::string_view> description,
        const std::vector<std::string>& files,
        std::optional<std::string_view> creator_id,
        const std::vector<std::string>& tags
    ) -> std::string {
        return connect().send<std::string>(
            event::add_post,
            description,
            files,
            creator_id,
            tags
        );
    }

    auto api::get_creator(std::string_view id) -> core::creator {
        return connect().send<core::creator>(
            event::get_creator,
            id
        );
    }

    auto api::get_post(std::string_view id) -> core::post {
        return connect().send<core::post>(
            event::get_post,
            id
        );
    }

    auto api::get_server_info() -> server::server_info {
        return connect().send<server::server_info>(event::get_server_info);
    }
}
