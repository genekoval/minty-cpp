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
            add_object_data,
            add_object_local,
            add_objects_url,
            add_post,
            add_post_objects,
            add_post_tag,
            add_tag,
            add_tag_alias,
            add_tag_source,
            delete_post,
            delete_post_tag,
            delete_tag,
            delete_tag_alias,
            delete_tag_source,
            get_comments,
            get_object,
            get_post,
            get_server_info,
            get_tag,
            get_tag_posts,
            get_tag_previews,
            get_tags_by_name,
            set_tag_description,
            set_tag_name
        };

        using client = net::client<event>;

        const std::string endpoint;
        const net::error_list errors;

        auto connect() -> client;
    public:
        api(std::string_view endpoint);

        auto add_post(
            std::optional<std::string_view> description,
            const std::vector<std::string>& files,
            std::optional<std::string_view> tag_id,
            const std::vector<std::string>& tags
        ) -> std::string;

        auto add_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        auto add_tag(std::string_view name) -> std::string;

        auto add_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> core::tag_name;

        auto add_tag_source(
            std::string_view tag_id,
            std::string_view url
        ) -> core::source;

        auto delete_post(std::string_view id) -> void;

        auto delete_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        auto delete_tag(std::string_view id) -> void;

        auto delete_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> core::tag_name;

        auto delete_tag_source(
            std::string_view tag_id,
            std::string_view source_id
        ) -> void;

        auto get_comments(
            std::string_view post_id
        ) -> std::vector<core::comment>;

        auto get_object(std::string_view object_id) -> core::object;

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
