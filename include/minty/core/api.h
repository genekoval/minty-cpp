#pragma once

#include <minty/core/model.h>
#include <minty/repo/db/database.h>

#include <fstore/client.h>

namespace minty::core {
    class api {
        repo::db::database* db;
        fstore::bucket* bucket;

        auto add_post(
            std::string_view description,
            const std::vector<std::string>& objects,
            std::optional<std::string_view> creator_id,
            const std::vector<std::string>& tags
        ) -> std::string;

        auto get_object_metadata(
            std::span<const repo::db::object> db_objects
        ) -> std::vector<object>;
    public:
        api(repo::db::database& db, fstore::bucket& bucket);

        auto add_creator(std::string_view name) -> std::string;

        auto add_post(
            std::string_view description,
            std::span<std::span<const std::byte>> data,
            std::optional<std::string_view> creator_id,
            const std::vector<std::string>& tags
        ) -> std::string;

        auto add_post(
            std::string_view description,
            std::span<std::string> files,
            std::optional<std::string_view> creator_id,
            const std::vector<std::string>& tags
        ) -> std::string;

        auto add_tag(std::string_view name, std::string_view color) -> tag;

        auto get_creator(std::string_view id) -> creator;

        auto get_creator_posts(
            std::string_view creator_id
        ) -> std::vector<post_preview>;

        auto get_creator_previews() -> std::vector<creator_preview>;

        auto get_post(std::string_view id) -> post;
    };
}
