#pragma once

#include "event.h"

#include <minty/core/model.h>
#include <minty/server/server_info.h>

#include <netcore/netcore>
#include <zipline/zipline>

namespace minty {
    class api {
        using client = net::client<event>;

        const net::error_list errors;
        const std::function<netcore::socket()> socket;

        auto connect() -> client;
    public:
        api(std::string_view endpoint);

        auto add_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) -> core::comment;

        auto add_object_data(
            const std::filesystem::path& path
        ) -> core::object_preview;

        auto add_object_local(std::string_view path) -> core::object_preview;

        auto add_objects(
            std::span<const std::string_view> arguments
        ) -> std::vector<core::object_preview>;

        auto add_objects_url(
            std::string_view url
        ) -> std::vector<core::object_preview>;

        auto add_post(const core::post_parts& parts) -> UUID::uuid;

        auto add_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            std::int16_t position
        ) -> decltype(core::post::date_modified);

        auto add_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> void;

        auto add_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        auto add_reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) -> core::comment;

        auto add_tag(std::string_view name) -> UUID::uuid;

        auto add_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> core::tag_name;

        auto add_tag_source(
            const UUID::uuid& tag_id,
            std::string_view url
        ) -> core::source;

        auto delete_post(const UUID::uuid& id) -> void;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects
        ) -> decltype(core::post::date_modified);

        auto delete_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> void;

        auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        auto delete_tag(std::string_view id) -> void;

        auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> core::tag_name;

        auto delete_tag_source(
            const UUID::uuid& tag_id,
            std::string_view source_id
        ) -> void;

        auto get_comment(const UUID::uuid& comment_id) -> core::comment_detail;

        auto get_comments(
            const UUID::uuid& post_id
        ) -> std::vector<core::comment>;

        auto get_object(const UUID::uuid& object_id) -> core::object;

        auto get_post(const UUID::uuid& id) -> core::post;

        auto get_posts(
            const core::post_query& query
        ) -> core::search_result<core::post_preview>;

        auto get_server_info() -> server::server_info;

        auto get_tag(const UUID::uuid& id) -> core::tag;

        auto get_tags(
            const core::tag_query& query
        ) -> core::search_result<core::tag_preview>;

        auto move_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            const std::optional<UUID::uuid>& destination
        ) -> decltype(core::post::date_modified);

        auto set_comment_content(
            const UUID::uuid& comment_id,
            std::string_view content
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
            const UUID::uuid& tag_id,
            std::string_view description
        ) -> std::optional<std::string>;

        auto set_tag_name(
            const UUID::uuid& tag_id,
            std::string_view new_name
        ) -> core::tag_name;
    };
}
