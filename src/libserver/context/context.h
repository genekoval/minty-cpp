#pragma once

#include <minty/core/api.h>
#include <minty/net/zipline/protocol.h>
#include <minty/server/server_info.h>

namespace minty::server {
    class context {
        core::api* api;
        const server_info* info;
    public:
        context(core::api& api, const server_info& info);

        auto add_comment(
            UUID::uuid post_id,
            std::string content
        ) -> core::comment;

        auto add_object_data(net::data_stream stream) -> core::object_preview;

        auto add_object_local(std::string path) -> core::object_preview;

        auto add_objects_url(
            std::string url
        ) -> std::vector<core::object_preview>;

        auto add_post(core::post_parts parts) -> UUID::uuid;

        auto add_post_objects(
            UUID::uuid post_id,
            std::vector<UUID::uuid> objects,
            std::int16_t position
        ) -> std::string;

        auto add_post_tag(UUID::uuid post_id, UUID::uuid tag_id) -> void;

        auto add_related_post(UUID::uuid post_id, UUID::uuid related) -> void;

        auto add_reply(
            UUID::uuid parent_id,
            std::string content
        ) -> core::comment;

        auto add_tag(std::string name) -> UUID::uuid;

        auto add_tag_alias(
            UUID::uuid tag_id,
            std::string alias
        ) -> core::tag_name;

        auto add_tag_source(
            UUID::uuid tag_id,
            std::string url
        ) -> core::source;

        auto delete_post(UUID::uuid post_id) -> void;

        auto delete_post_objects(
            UUID::uuid post_id,
            std::vector<UUID::uuid> objects
        ) -> std::string;

        auto delete_post_objects_ranges(
            UUID::uuid post_id,
            std::vector<core::range> ranges
        ) -> std::string;

        auto delete_post_tag(
            UUID::uuid post_id,
            UUID::uuid tag_id
        ) -> void;

        auto delete_related_post(
            UUID::uuid post_id,
            UUID::uuid related
        ) -> void;

        auto delete_tag(UUID::uuid tag_id) -> void;

        auto delete_tag_alias(
            UUID::uuid tag_id,
            std::string alias
        ) -> core::tag_name;

        auto delete_tag_source(
            UUID::uuid tag_id,
            std::string source_id
        ) -> void;

        auto get_comment(UUID::uuid comment_id) -> core::comment_detail;

        auto get_comments(UUID::uuid post_id) -> core::comment_tree;

        auto get_object(UUID::uuid object_id) -> core::object;

        auto get_post(UUID::uuid post_id) -> core::post;

        auto get_posts(
            core::post_query query
        ) -> core::search_result<core::post_preview>;

        auto get_server_info() -> server_info;

        auto get_tag(UUID::uuid tag_id) -> core::tag;

        auto get_tags(
            core::tag_query query
        ) -> core::search_result<core::tag_preview>;

        auto move_post_object(
            UUID::uuid post_id,
            std::uint32_t old_index,
            std::uint32_t new_index
        ) -> void;

        auto move_post_objects(
            UUID::uuid post_id,
            std::vector<UUID::uuid> objects,
            std::optional<UUID::uuid> destination
        ) -> std::string;

        auto set_comment_content(
            UUID::uuid comment_id,
            std::string content
        ) -> std::string;

        auto set_post_description(
            UUID::uuid post_id,
            std::string description
        ) -> core::modification<std::optional<std::string>>;

        auto set_post_title(
            UUID::uuid post_id,
            std::string title
        ) -> core::modification<std::optional<std::string>>;

        auto set_tag_description(
            UUID::uuid tag_id,
            std::string description
        ) -> std::optional<std::string>;

        auto set_tag_name(
            UUID::uuid tag_id,
            std::string new_name
        ) -> core::tag_name;
    };
}
