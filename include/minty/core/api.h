#pragma once

#include <minty/core/comment_tree.h>
#include <minty/core/model.h>
#include <minty/core/preview.h>
#include <minty/repo/db/database.h>

#include <fstore/client.h>

namespace minty::core {
    class downloader;
    class search_engine;

    class api {
        repo::db::database* db;
        fstore::bucket* bucket;
        downloader* dl;
        preview_service previews;
        search_engine* search;

        auto get_object_metadata(
            std::span<const repo::db::object> db_objects
        ) -> std::vector<object>;
    public:
        api(
            repo::db::database& db,
            fstore::bucket& bucket,
            downloader& dl,
            search_engine& search
        );

        auto add_comment(
            std::string_view post_id,
            std::optional<std::string_view> parent_id,
            std::string_view content
        ) -> comment;

        auto add_object_data(
            std::size_t stream_size,
            std::function<void(fstore::part&&)> pipe
        ) -> std::string;

        auto add_object_local(std::string_view path) -> std::string;

        auto add_object_url(std::string_view url) -> std::vector<std::string>;

        auto add_post(post_parts parts) -> std::string;

        auto add_tag(std::string_view name) -> std::string;

        auto add_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> tag_name;

        auto delete_post(std::string_view id) -> void;

        auto delete_tag(std::string_view id) -> void;

        auto delete_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> tag_name;

        auto get_comments(std::string_view post_id) -> comment_tree;

        auto get_post(std::string_view id) -> post;

        auto get_tag(std::string_view id) -> tag;

        auto get_tags_by_name(
            std::string_view term
        ) -> std::vector<tag_preview>;

        auto get_tag_posts(
            std::string_view tag_id
        ) -> std::vector<post_preview>;

        auto get_tag_previews() -> std::vector<tag_preview>;

        auto set_tag_description(
            std::string_view tag_id,
            std::string_view description
        ) -> std::optional<std::string>;

        auto set_tag_name(
            std::string_view tag_id,
            std::string_view new_name
        ) -> tag_name;
    };
}
