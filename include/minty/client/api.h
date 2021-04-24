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
            add_tag_alias,
            delete_post,
            delete_tag,
            delete_tag_alias,
            get_comments,
            get_post,
            get_server_info,
            get_tag,
            get_tags_by_name,
            get_tag_posts,
            get_tag_previews,
            set_tag_description,
            set_tag_name
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

        auto add_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> core::tag_name;

        auto delete_post(std::string_view id) -> void;

        auto delete_tag(std::string_view id) -> void;

        auto delete_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> core::tag_name;

        auto get_comments(
            std::string_view post_id
        ) -> std::vector<core::comment>;

        auto get_post(std::string_view id) -> core::post;

        auto get_server_info() -> server::server_info;

        auto get_tag(std::string_view id) -> core::tag;

        auto get_tags_by_name(
            std::string_view search_term
        ) -> std::vector<core::tag_preview>;

        auto set_tag_description(
            std::string_view tag_id,
            std::string_view description
        ) -> std::optional<std::string>;

        auto set_tag_name(
            std::string_view tag_id,
            std::string_view new_name
        ) -> core::tag_name;
    };
}
