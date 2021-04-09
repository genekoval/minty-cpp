#pragma once

#include <minty/net/zipline/post_parts.h>
#include <minty/server/server.h>

namespace minty::server::route {
    auto add_comment(
        context* ctx,
        std::string post_id,
        std::optional<std::string> parent_id,
        std::string content
    ) -> minty::core::comment;

    auto add_post(context* ctx, net::post_parts parts) -> std::string;

    auto add_tag(context* ctx, std::string name) -> std::string;

    auto delete_post(context* ctx, std::string post_id) -> void;

    auto get_comments(context* ctx, std::string post_id) -> core::comment_tree;

    auto get_post(context* ctx, std::string post_id) -> core::post;

    auto get_server_info(context* ctx) -> server_info;

    auto get_tag(context* ctx, std::string tag_id) -> core::tag;

    auto get_tags_by_name(
        context* ctx,
        std::string search_term
    ) -> std::vector<core::tag_preview>;

    auto get_tag_posts(
        context* ctx,
        std::string tag_id
    ) -> std::vector<core::post_preview>;

    auto get_tag_previews(context* ctx) -> std::vector<core::tag_preview>;
}
