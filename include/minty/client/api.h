#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>
#include <minty/server/server_info.h>

#include <netcore/socket.h>
#include <zipline/zipline>

namespace minty {
    class api {
        enum class event : net::event_t {
            add_comment,
            add_creator,
            add_post,
            get_comments,
            get_creator,
            get_creator_posts,
            get_creator_previews,
            get_post,
            get_server_info
        };

        using protocol = zipline::protocol<net::socket>;
        using client = zipline::client<protocol, event>;

        const std::string endpoint;

        auto connect() -> client;
    public:
        api(std::string_view endpoint);

        auto add_creator(std::string_view name) -> std::string;

        auto add_post(
            std::optional<std::string_view> description,
            const std::vector<std::string>& files,
            std::optional<std::string_view> creator_id,
            const std::vector<std::string>& tags
        ) -> std::string;

        auto get_creator(std::string_view id) -> core::creator;

        auto get_server_info() -> server::server_info;
    };
}
