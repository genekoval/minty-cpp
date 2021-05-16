#pragma once

#include <minty/core/api.h>
#include <minty/net/zipline/post_parts.h>
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

        auto add_post(net::post_parts parts) -> std::string;

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

        auto get_post(std::string post_id) -> core::post;

        auto get_server_info() -> server_info;

        auto get_tag(std::string tag_id) -> core::tag;

        auto get_tag_posts(
            std::string tag_id
        ) -> std::vector<core::post_preview>;

        auto get_tag_previews() -> std::vector<core::tag_preview>;

        auto get_tags_by_name(
            std::string search_term
        ) -> std::vector<core::tag_preview>;

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
