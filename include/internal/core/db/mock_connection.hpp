#pragma once

#include "connection.hpp"

#include <gmock/gmock.h>

namespace minty::core::db {
    struct mock_connection : connection {
        auto begin() -> ext::task<pg::transaction> override {
            co_return pg::transaction();
        }

        MOCK_METHOD(ext::task<comment>, create_comment, (
            const UUID::uuid& post_id,
            std::string_view content
        ), (override));

        MOCK_METHOD(ext::task<>, create_object, (
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id,
            const std::optional<std::int64_t>& source_id
        ), (override));

        MOCK_METHOD(ext::task<>, create_object_preview_error, (
            const UUID::uuid& object_id,
            std::string_view message
        ), (override));

        MOCK_METHOD(ext::task<time_point>, create_post, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(ext::task<post_search>, create_post_draft, (), (override));

        MOCK_METHOD(ext::task<time_point>, create_post_objects, (
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            const std::optional<UUID::uuid>& destination
        ), (override));

        MOCK_METHOD(ext::task<>, create_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::task<comment>, create_reply, (
            const UUID::uuid& parent_id,
            std::string_view content
        ), (override));

        MOCK_METHOD(ext::task<>, create_related_post, (
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ), (override));

        MOCK_METHOD(ext::task<site>, create_site, (
            std::string_view scheme,
            std::string_view name,
            std::optional<UUID::uuid> icon
        ), (override));

        MOCK_METHOD(ext::task<source>, create_source, (
            std::int64_t site_id,
            std::string_view resource
        ), (override));

        MOCK_METHOD(ext::task<UUID::uuid>, create_tag, (
            std::string_view name
        ), (override));

        MOCK_METHOD(ext::task<tag_name>, create_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(ext::task<>, create_tag_source, (
            const UUID::uuid& tag_id,
            std::int64_t source_id
        ), (override));

        MOCK_METHOD(ext::task<>, delete_object_preview_error, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(ext::task<>, delete_post, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(
            ext::task<decltype(post::date_modified)>, delete_post_objects, (
                const UUID::uuid& post_id,
                const std::vector<UUID::uuid>& objects
            ), (override)
        );

        MOCK_METHOD(ext::task<>, delete_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::task<>, delete_related_post, (
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ), (override));

        MOCK_METHOD(ext::task<>, delete_tag, (
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::task<tag_name>, delete_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(ext::task<>, delete_tag_source, (
            const UUID::uuid& tag_id,
            std::int64_t source_id
        ), (override));

        MOCK_METHOD(ext::task<>, prune, (), (override));

        MOCK_METHOD(ext::task<std::vector<UUID::uuid>>, prune_objects, (
        ), (override));

        MOCK_METHOD(ext::task<comment>, read_comment, (
            const UUID::uuid& comment_id
        ), (override));

        MOCK_METHOD(ext::task<std::vector<comment>>, read_comments, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(ext::task<object>, read_object, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(ext::task<std::vector<post_preview>>, read_object_posts, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(
            ext::task<std::vector<object_error>>,
            read_object_preview_errors, (), (override)
        );

        MOCK_METHOD(ext::task<pg::portal<object_preview>>, read_objects, (
            int batch_size
        ), (override));

        MOCK_METHOD(ext::task<post>, read_post, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(ext::task<std::vector<post_preview>>, read_posts, (
            const std::vector<UUID::uuid>& posts
        ), (override));

        MOCK_METHOD(ext::task<pg::portal<post_search>>, read_post_search, (
            int batch_size
        ), (override));

        MOCK_METHOD(ext::task<std::vector<tag_preview>>, read_post_tags, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(ext::task<std::vector<post_preview>>, read_related_posts, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(ext::task<std::optional<std::int64_t>>, read_site, (
            std::string_view scheme,
            std::string_view host
        ), (override));

        MOCK_METHOD(ext::task<tag>, read_tag, (
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::task<std::vector<tag_preview>>, read_tag_previews, (
            const std::vector<UUID::uuid>& tags
        ), (override));

        MOCK_METHOD(ext::task<std::vector<source>>, read_tag_sources, (
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::task<pg::portal<tag_search>>, read_tag_search, (
            int batch_size
        ), (override));

        MOCK_METHOD(ext::task<std::size_t>, read_total_objects, (), (override));

        MOCK_METHOD(ext::task<>, update_comment, (
            const UUID::uuid& comment_id,
            std::string_view content
        ), (override));

        MOCK_METHOD(ext::task<>, update_object_preview, (
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id
        ), (override));

        MOCK_METHOD(ext::task<post_update>, update_post_description, (
            const UUID::uuid& post_id,
            std::string_view description
        ), (override));

        MOCK_METHOD(ext::task<post_update>, update_post_title, (
            const UUID::uuid& post_id,
            std::string_view title
        ), (override));

        MOCK_METHOD(
            ext::task<std::optional<std::string>>, update_tag_description, (
                const UUID::uuid& tag_id,
                std::string_view description
            ), (override)
        );

        MOCK_METHOD(ext::task<tag_name_update>, update_tag_name, (
            const UUID::uuid& tag_id,
            std::string_view name
        ), (override));
    };
}
