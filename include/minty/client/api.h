#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>
#include <minty/server/server_info.h>

#include <netcore/netcore>
#include <zipline/zipline>

namespace minty {
    class api {
        enum class event : net::event_t {
            add_comment,
            add_post,
            add_tag,
            get_comments,
            get_post,
            get_server_info,
            get_tag,
            get_tag_posts,
            get_tag_previews
        };

        using protocol = zipline::protocol<net::socket>;
        using client = zipline::client<protocol, event>;

        const std::string endpoint;

        auto connect() -> client;
    public:
        api(std::string_view endpoint);

        auto add_post(
            std::optional<std::string_view> description,
            const std::vector<std::string>& files,
            std::optional<std::string_view> tag_id,
            const std::vector<std::string>& tags
        ) -> std::string;

        auto add_tag(std::string_view name) -> std::string;

        auto get_comments(
            std::string_view post_id
        ) -> std::vector<core::comment>;

        auto get_post(std::string_view id) -> core::post;

        auto get_server_info() -> server::server_info;

        auto get_tag(std::string_view id) -> core::tag;
    };
}
