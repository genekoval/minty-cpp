#pragma once

#include "database.hpp"

#include <gmock/gmock.h>

namespace minty::test {
    struct database : core::db::database {
        MOCK_METHOD(comment, create_comment, (
            const UUID::uuid& post_id,
            std::string_view content
        ), (override));

        MOCK_METHOD(void, create_object, (
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id,
            const std::optional<std::string_view>& source_id
        ), (override));

        MOCK_METHOD(void, create_object_preview_error, (
            const UUID::uuid& object_id,
            std::string_view message
        ), (override));

        MOCK_METHOD(core::db::post_search, create_post, (
            std::string_view title,
            std::string_view description,
            const std::vector<UUID::uuid>& objects,
            const std::vector<UUID::uuid>& tags
        ), (override));

        MOCK_METHOD(time_point, create_post_objects, (
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            std::int16_t position
        ), (override));

        MOCK_METHOD(void, create_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(void, create_related_post, (
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ), (override));

        MOCK_METHOD(comment, create_reply, (
            const UUID::uuid& parent_id,
            std::string_view content
        ), (override));

        MOCK_METHOD(core::db::site, create_site, (
            std::string_view scheme,
            std::string_view name,
            std::optional<UUID::uuid> icon
        ), (override));

        MOCK_METHOD(core::db::source, create_source, (
            std::string_view site_id,
            std::string_view resource
        ), (override));

        MOCK_METHOD(UUID::uuid, create_tag, (
            std::string_view name
        ), (override));

        MOCK_METHOD(tag_name, create_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(void, create_tag_source, (
            const UUID::uuid& tag_id,
            std::string_view source_id
        ), (override));

        MOCK_METHOD(void, delete_object_preview_error, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(void, delete_post, (const UUID::uuid& post_id), (override));

        MOCK_METHOD(time_point, delete_post_objects, (
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects
        ), (override));

        MOCK_METHOD(time_point, delete_post_objects_ranges, (
            const UUID::uuid& post_id,
            std::span<const range> ranges
        ), (override));

        MOCK_METHOD(void, delete_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(void, delete_related_post, (
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ), (override));

        MOCK_METHOD(void, delete_tag, (const UUID::uuid& tag_id), (override));

        MOCK_METHOD(tag_name, delete_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(void, delete_tag_source, (
            const UUID::uuid& tag_id,
            std::string_view source_id
        ), (override));

        MOCK_METHOD(time_point, move_post_object, (
            const UUID::uuid& post_id,
            unsigned int old_index,
            unsigned int new_index
        ), (override));

        MOCK_METHOD(time_point, move_post_objects, (
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            const std::optional<UUID::uuid>& destination
        ), (override));

        MOCK_METHOD(void, prune, (), (override));

        MOCK_METHOD(ext::task<>, prune_objects, (
            const std::function<ext::task<bool>(
                std::span<const UUID::uuid>
            )>& on_deleted
        ), (override));

        MOCK_METHOD(comment, read_comment, (
            const UUID::uuid& comment_id
        ), (override));

        MOCK_METHOD(std::vector<comment>, read_comments, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(core::db::object, read_object, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(std::vector<core::db::post_preview>, read_object_posts, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(
            std::vector<object_error>,
            read_object_preview_errors, (),
            (override)
        );

        MOCK_METHOD(
            ext::generator<std::span<core::db::object_preview>>,
            read_objects, (int batch_size), (override)
        );

        MOCK_METHOD(core::db::post, read_post, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(std::vector<core::db::post_preview>, read_posts, (
            const std::vector<UUID::uuid>& posts
        ), (override));

        MOCK_METHOD(std::vector<core::db::object_preview>, read_post_objects, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(ext::generator<std::span<core::db::post_search>>,
            read_post_search, (int batch_size), (override)
        );

        MOCK_METHOD(std::vector<tag_preview>, read_post_tags, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(std::vector<core::db::post_preview>, read_related_posts, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(std::optional<std::string>, read_site, (
            std::string_view scheme,
            std::string_view host
        ), (override));

        MOCK_METHOD(core::db::tag, read_tag, (
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(std::vector<tag_preview>, read_tag_previews, (
            const std::vector<UUID::uuid>& tags
        ), (override));

        MOCK_METHOD(std::vector<core::db::source>, read_tag_sources, (
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::generator<std::span<core::db::tag_search>>,
            read_tag_text, (int batch_size), (override)
        );

        MOCK_METHOD(std::size_t, read_total_objects, (), (override));

        MOCK_METHOD(void, update_comment, (
            const UUID::uuid& comment_id,
            std::string_view content
        ), (override));

        MOCK_METHOD(void, update_object_preview, (
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id
        ), (override));

        MOCK_METHOD(core::db::post_update, update_post_description, (
            const UUID::uuid& post_id,
            std::string_view description
        ), (override));

        MOCK_METHOD(core::db::post_update, update_post_title, (
            const UUID::uuid& post_id,
            std::string_view title
        ), (override));

        MOCK_METHOD(std::optional<std::string>, update_tag_description, (
            const UUID::uuid& tag_id,
            std::string_view description
        ), (override));

        MOCK_METHOD(core::db::tag_name_update, update_tag_name, (
            const UUID::uuid& tag_id,
            std::string_view name
        ), (override));
    };
}
