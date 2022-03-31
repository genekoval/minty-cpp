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
            add_related_post,
            add_tag,
            add_tag_alias,
            add_tag_source,
            delete_post,
            delete_post_objects,
            delete_post_objects_ranges,
            delete_post_tag,
            delete_related_post,
            delete_tag,
            delete_tag_alias,
            delete_tag_source,
            get_comments,
            get_object,
            get_post,
            get_posts,
            get_server_info,
            get_tag,
            get_tags,
            move_post_object,
            move_post_objects,
            set_comment_content,
            set_post_description,
            set_post_title,
            set_tag_description,
            set_tag_name
        };

        using client = net::client<event>;

        const std::string endpoint;
        const net::error_list errors;

        auto connect() -> client;
    public:
        api(std::string_view endpoint);

        auto add_object_local(std::string_view path) -> std::string;

        auto add_post(const core::post_parts& parts) -> std::string;

        auto add_post_objects(
            std::string_view post_id,
            std::span<std::string> objects,
            std::int16_t position
        ) -> std::string;

        auto add_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        auto add_related_post(
            std::string_view post_id,
            std::string_view related
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

        auto delete_post_objects(
            std::string_view post_id,
            std::span<const std::string_view> objects
        ) -> std::string;

        auto delete_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        auto delete_related_post(
            std::string_view post_id,
            std::string_view related
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

        auto get_posts(
            const core::post_query& query
        ) -> core::search_result<core::post_preview>;

        auto get_server_info() -> server::server_info;

        auto get_tag(std::string_view id) -> core::tag;

        auto get_tags(
            const core::tag_query& query
        ) -> core::search_result<core::tag_preview>;

        auto set_post_description(
            std::string_view post_id,
            std::string_view description
        ) -> core::modification<std::optional<std::string>>;

        auto set_post_title(
            std::string_view post_id,
            std::string_view title
        ) -> core::modification<std::optional<std::string>>;

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
