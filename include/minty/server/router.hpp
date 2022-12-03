#pragma once

#include <minty/core/api.h>
#include <minty/net/zipline/protocol.h>
#include <minty/server/server_info.h>

namespace minty::server {
    class router_context {
        core::api* api;
        const server_info* info;
    public:
        router_context(core::api& api, const server_info& info);

        auto add_comment(
            UUID::uuid post_id,
            std::string content
        ) -> ext::task<core::comment>;

        auto add_object_data(
            net::data_stream stream
        ) -> ext::task<core::object_preview>;

        auto add_objects_url(
            std::string url
        ) -> ext::task<std::vector<core::object_preview>>;

        auto add_post(core::post_parts parts) -> ext::task<UUID::uuid>;

        auto add_post_objects(
            UUID::uuid post_id,
            std::vector<UUID::uuid> objects,
            std::int16_t position
        ) -> ext::task<decltype(core::post::date_modified)>;

        auto add_post_tag(UUID::uuid post_id, UUID::uuid tag_id) -> ext::task<>;

        auto add_related_post(
            UUID::uuid post_id,
            UUID::uuid related
        ) -> ext::task<>;

        auto add_reply(
            UUID::uuid parent_id,
            std::string content
        ) -> ext::task<core::comment>;

        auto add_tag(std::string name) -> ext::task<UUID::uuid>;

        auto add_tag_alias(
            UUID::uuid tag_id,
            std::string alias
        ) -> ext::task<core::tag_name>;

        auto add_tag_source(
            UUID::uuid tag_id,
            std::string url
        ) -> ext::task<core::source>;

        auto delete_post(UUID::uuid post_id) -> ext::task<>;

        auto delete_post_objects(
            UUID::uuid post_id,
            std::vector<UUID::uuid> objects
        ) -> ext::task<decltype(core::post::date_modified)>;

        auto delete_post_objects_ranges(
            UUID::uuid post_id,
            std::vector<core::range> ranges
        ) -> ext::task<decltype(core::post::date_modified)>;

        auto delete_post_tag(
            UUID::uuid post_id,
            UUID::uuid tag_id
        ) -> ext::task<>;

        auto delete_related_post(
            UUID::uuid post_id,
            UUID::uuid related
        ) -> ext::task<>;

        auto delete_tag(UUID::uuid tag_id) -> ext::task<>;

        auto delete_tag_alias(
            UUID::uuid tag_id,
            std::string alias
        ) -> ext::task<core::tag_name>;

        auto delete_tag_source(
            UUID::uuid tag_id,
            std::string source_id
        ) -> ext::task<>;

        auto get_comment(
            UUID::uuid comment_id
        ) -> ext::task<core::comment_detail>;

        auto get_comments(UUID::uuid post_id) -> ext::task<core::comment_tree>;

        auto get_object(UUID::uuid object_id) -> ext::task<core::object>;

        auto get_post(UUID::uuid post_id) -> ext::task<core::post>;

        auto get_posts(
            core::post_query query
        ) -> ext::task<core::search_result<core::post_preview>>;

        auto get_server_info() -> ext::task<server_info>;

        auto get_tag(UUID::uuid tag_id) -> ext::task<core::tag>;

        auto get_tags(
            core::tag_query query
        ) -> ext::task<core::search_result<core::tag_preview>>;

        auto move_post_object(
            UUID::uuid post_id,
            std::uint32_t old_index,
            std::uint32_t new_index
        ) -> ext::task<>;

        auto move_post_objects(
            UUID::uuid post_id,
            std::vector<UUID::uuid> objects,
            std::optional<UUID::uuid> destination
        ) -> ext::task<decltype(core::post::date_modified)>;

        auto set_comment_content(
            UUID::uuid comment_id,
            std::string content
        ) -> ext::task<std::string>;

        auto set_post_description(
            UUID::uuid post_id,
            std::string description
        ) -> ext::task<core::modification<std::optional<std::string>>>;

        auto set_post_title(
            UUID::uuid post_id,
            std::string title
        ) -> ext::task<core::modification<std::optional<std::string>>>;

        auto set_tag_description(
            UUID::uuid tag_id,
            std::string description
        ) -> ext::task<std::optional<std::string>>;

        auto set_tag_name(
            UUID::uuid tag_id,
            std::string new_name
        ) -> ext::task<core::tag_name>;
    };
}
