#pragma once

#include <minty/model/post_query.hpp>
#include <minty/model/tag_query.hpp>

#include <http/http>

namespace minty::detail::http {
    using request = ::http::client::request;

    class repo {
        const ::http::client client;
    public:
        repo(std::string_view base_url);

        repo(std::string_view base_url, ::http::session& session);

        auto add_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) const -> request;

        auto add_object_data(
            const std::filesystem::path& path
        ) const -> request;

        auto add_objects_url(
            std::string_view url
        ) const -> request;

        auto add_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects
        ) const -> request;

        auto add_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) const -> request;

        auto add_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) const -> request;

        auto add_tag(std::string_view name) const -> request;

        auto add_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) const -> request;

        auto add_tag_source(
            const UUID::uuid& tag_id,
            std::string_view url
        ) const -> request;

        auto create_post(const UUID::uuid& draft) const -> request;

        auto create_post_draft() const -> request;

        auto delete_post(const UUID::uuid& id) const -> request;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects
        ) const -> request;

        auto delete_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) const -> request;

        auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) const -> request;

        auto delete_tag(const UUID::uuid& id) const -> request;

        auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) const -> request;

        auto delete_tag_source(
            const UUID::uuid& tag_id,
            std::int64_t source_id
        ) const -> request;

        auto get_comment(const UUID::uuid& comment_id) const -> request;

        auto get_comments(
            const UUID::uuid& post_id
        ) const -> request;

        auto get_object(const UUID::uuid& object_id) const -> request;

        auto get_post(const UUID::uuid& id) const -> request;

        auto get_posts(
            const post_query& query
        ) const -> request;

        auto get_server_info() const -> request;

        auto get_tag(const UUID::uuid& id) const -> request;

        auto get_tags(
            const tag_query& query
        ) const -> request;

        auto insert_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            const UUID::uuid& destination
        ) const -> request;

        auto reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) const -> request;

        auto set_comment_content(
            const UUID::uuid& comment_id,
            std::string_view content
        ) const -> request;

        auto set_post_description(
            const UUID::uuid& post_id,
            std::string_view description
        ) const -> request;

        auto set_post_title(
            const UUID::uuid& post_id,
            std::string_view title
        ) const -> request;

        auto set_tag_description(
            const UUID::uuid& tag_id,
            std::string_view description
        ) const -> request;

        auto set_tag_name(
            const UUID::uuid& tag_id,
            std::string_view new_name
        ) const -> request;
    };
}
