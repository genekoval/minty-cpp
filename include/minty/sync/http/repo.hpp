#pragma once

#include <minty/detail/http/repo.hpp>
#include <minty/model/comment.hpp>
#include <minty/model/modification.hpp>
#include <minty/model/object.hpp>
#include <minty/model/object_preview.hpp>
#include <minty/model/post.hpp>
#include <minty/model/search_result.hpp>
#include <minty/model/server_info.hpp>
#include <minty/model/source.hpp>
#include <minty/model/tag.hpp>
#include <minty/model/tag_name.hpp>
#include <minty/model/tag_preview.hpp>

#include <fstore/fstore>

namespace minty::sync::http {
    class repo {
        const detail::http::repo client;
        std::variant<std::monostate, std::string, fstore::sync::http::bucket>
            bucket_storage;

        auto bucket() -> fstore::sync::http::bucket&;
    public:
        repo(std::string_view base_url);

        repo(std::string_view base_url, std::string_view objects_url);

        auto add_comment(const UUID::uuid& post_id, std::string_view content)
            -> comment_data;

        auto add_object_data(const std::filesystem::path& path)
            -> object_preview;

        auto add_objects(std::span<const std::string_view> arguments)
            -> std::vector<object_preview>;

        auto add_objects_url(std::string_view url)
            -> std::vector<object_preview>;

        auto add_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects
        ) -> time_point;

        auto add_post_tag(const UUID::uuid& post_id, const UUID::uuid& tag_id)
            -> void;

        auto add_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        auto add_tag(std::string_view name) -> UUID::uuid;

        auto add_tag_alias(const UUID::uuid& tag_id, std::string_view alias)
            -> tag_name;

        auto add_tag_source(const UUID::uuid& tag_id, std::string_view url)
            -> source;

        auto create_post(const UUID::uuid& draft) -> void;

        auto create_post_draft() -> UUID::uuid;

        auto delete_post(const UUID::uuid& id) -> void;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects
        ) -> time_point;

        auto delete_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> void;

        auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        auto delete_tag(const UUID::uuid& id) -> void;

        auto delete_tag_alias(const UUID::uuid& tag_id, std::string_view alias)
            -> tag_name;

        auto delete_tag_source(const UUID::uuid& tag_id, std::int64_t source_id)
            -> void;

        auto download_object(
            const UUID::uuid& object_id,
            const std::filesystem::path& location
        ) -> void;

        auto get_comment(const UUID::uuid& comment_id)
            -> std::optional<comment>;

        auto get_comments(const UUID::uuid& post_id)
            -> std::vector<comment_data>;

        auto get_object(const UUID::uuid& object_id) -> std::optional<object>;

        auto get_object_data(const UUID::uuid& object_id) -> std::string;

        auto get_object_data(const UUID::uuid& object_id, FILE* file) -> void;

        auto get_post(const UUID::uuid& id) -> std::optional<post>;

        auto get_posts(const post_query& query) -> search_result<post_preview>;

        auto get_server_info() -> server_info;

        auto get_tag(const UUID::uuid& id) -> std::optional<tag>;

        auto get_tags(const tag_query& query) -> search_result<tag_preview>;

        auto insert_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            const UUID::uuid& destination
        ) -> time_point;

        auto reply(const UUID::uuid& parent_id, std::string_view content)
            -> comment_data;

        auto set_comment_content(
            const UUID::uuid& comment_id,
            std::string_view content
        ) -> std::string;

        auto set_post_description(
            const UUID::uuid& post_id,
            std::string_view description
        ) -> modification<std::optional<std::string>>;

        auto set_post_title(const UUID::uuid& post_id, std::string_view title)
            -> modification<std::optional<std::string>>;

        auto set_tag_description(
            const UUID::uuid& tag_id,
            std::string_view description
        ) -> std::optional<std::string>;

        auto set_tag_name(const UUID::uuid& tag_id, std::string_view new_name)
            -> tag_name;
    };
}
