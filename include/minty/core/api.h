#pragma once

#include <minty/core/model.h>
#include <minty/core/comment_tree.h>
#include <minty/repo/db/database.h>

#include <fstore/client.h>

namespace minty::core {
    class api {
        repo::db::database* db;
        fstore::bucket* bucket;

        auto get_object_metadata(
            std::span<const repo::db::object> db_objects
        ) -> std::vector<object>;
    public:
        api(repo::db::database& db, fstore::bucket& bucket);

        auto add_comment(
            std::string_view post_id,
            std::optional<std::string_view> parent_id,
            std::string_view content
        ) -> comment;

        auto add_creator(std::string_view name) -> std::string;

        auto add_object_data(
            std::size_t stream_size,
            std::function<void(fstore::part&&)> pipe
        ) -> std::string;

        auto add_object_local(std::string_view path) -> std::string;

        auto add_object_url(std::string_view url) -> std::string;

        auto add_post(post_parts parts) -> std::string;

        auto add_tag(std::string_view name, std::string_view color) -> tag;

        auto get_comments(std::string_view post_id) -> comment_tree;

        auto get_creator(std::string_view id) -> creator;

        auto get_creator_posts(
            std::string_view creator_id
        ) -> std::vector<post_preview>;

        auto get_creator_previews() -> std::vector<creator_preview>;

        auto get_post(std::string_view id) -> post;
    };
}
