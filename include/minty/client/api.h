#pragma once

#include "event.h"

#include <minty/core/model.h>
#include <minty/server/server_info.h>

#include <netcore/netcore>
#include <zipline/zipline>

namespace minty {
    class api {
        using client = net::client<event>;

        const std::string endpoint;
        const net::error_list errors;

        auto connect() -> client;
    public:
        api(std::string_view endpoint);

        auto add_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) -> core::comment;

        auto add_object_local(std::string_view path) -> core::object_preview;

        auto add_post(const core::post_parts& parts) -> UUID::uuid;

        auto add_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            std::int16_t position
        ) -> std::string;

        auto add_post_tag(
            const UUID::uuid& post_id,
            std::string_view tag_id
        ) -> void;

        auto add_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        auto add_reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) -> core::comment;

        auto add_tag(std::string_view name) -> std::string;

        auto add_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> core::tag_name;

        auto add_tag_source(
            std::string_view tag_id,
            std::string_view url
        ) -> core::source;

        auto delete_post(const UUID::uuid& id) -> void;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects
        ) -> std::string;

        auto delete_post_tag(
            const UUID::uuid& post_id,
            std::string_view tag_id
        ) -> void;

        auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
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
            const UUID::uuid& post_id
        ) -> std::vector<core::comment>;

        auto get_object(const UUID::uuid& object_id) -> core::object;

        auto get_post(const UUID::uuid& id) -> core::post;

        auto get_posts(
            const core::post_query& query
        ) -> core::search_result<core::post_preview>;

        auto get_server_info() -> server::server_info;

        auto get_tag(std::string_view id) -> core::tag;

        auto get_tags(
            const core::tag_query& query
        ) -> core::search_result<core::tag_preview>;

        auto move_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            const std::optional<UUID::uuid>& destination
        ) -> std::string;

        auto set_post_description(
            const UUID::uuid& post_id,
            std::string_view description
        ) -> core::modification<std::optional<std::string>>;

        auto set_post_title(
            const UUID::uuid& post_id,
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
