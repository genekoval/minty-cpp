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
            std::string post_id,
            std::optional<std::string> parent_id,
            std::string content
        ) -> core::comment;

        auto add_object_data(net::data_stream stream) -> std::string;

        auto add_object_local(std::string path) -> std::string;

        auto add_objects_url(std::string url) -> std::vector<std::string>;

        auto add_post(core::post_parts parts) -> std::string;

        auto add_post_objects(
            std::string post_id,
            std::vector<std::string> objects,
            unsigned int position
        ) -> std::vector<core::object_preview>;

        auto add_post_tag(std::string post_id, std::string tag_id) -> void;

        auto add_tag(std::string name) -> std::string;

        auto add_tag_alias(
            std::string tag_id,
            std::string alias
        ) -> core::tag_name;

        auto add_tag_source(
            std::string tag_id,
            std::string url
        ) -> core::source;

        auto delete_post(std::string post_id) -> void;

        auto delete_post_objects(
            std::string post_id,
            std::vector<core::range> ranges
        ) -> void;

        auto delete_post_tag(
            std::string post_id,
            std::string tag_id
        ) -> void;

        auto delete_tag(std::string tag_id) -> void;

        auto delete_tag_alias(
            std::string tag_id,
            std::string alias
        ) -> core::tag_name;

        auto delete_tag_source(
            std::string tag_id,
            std::string source_id
        ) -> void;

        auto get_comments(std::string post_id) -> core::comment_tree;

        auto get_object(std::string object_id) -> core::object;

        auto get_post(std::string post_id) -> core::post;

        auto get_posts(
            core::post_query query
        ) -> core::search_result<core::post_preview>;

        auto get_server_info() -> server_info;

        auto get_tag(std::string tag_id) -> core::tag;

        auto get_tag_posts(
            std::string tag_id
        ) -> std::vector<core::post_preview>;

        auto get_tag_previews() -> std::vector<core::tag_preview>;

        auto get_tags(
            core::tag_query query
        ) -> core::search_result<core::tag_preview>;

        auto move_post_object(
            std::string post_id,
            std::uint32_t old_index,
            std::uint32_t new_index
        ) -> void;

        auto set_comment_content(
            std::string comment_id,
            std::string content
        ) -> std::string;

        auto set_post_description(
            std::string post_id,
            std::string description
        ) -> std::optional<std::string>;

        auto set_post_title(
            std::string post_id,
            std::string title
        ) -> std::optional<std::string>;

        auto set_tag_description(
            std::string tag_id,
            std::string description
        ) -> std::optional<std::string>;

        auto set_tag_name(
            std::string tag_id,
            std::string new_name
        ) -> core::tag_name;
    };
}
