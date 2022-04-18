#pragma once

#include <minty/core/search.h>

#include <gmock/gmock.h>

namespace minty::test {
    struct search_engine : core::search_engine {
        MOCK_METHOD(void, add_post, (
            const core::post_search& post
        ), (override));

        MOCK_METHOD(void, add_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(void, add_posts, (
            std::span<const core::post_search> posts
        ), (override));

        MOCK_METHOD(void, add_tags, (
            std::span<const core::tag_text> tags
        ), (override));

        MOCK_METHOD(void, add_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(void, create_indices, (), (override));

        MOCK_METHOD(void, delete_indices, (), (override));

        MOCK_METHOD(void, delete_post, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(void, delete_tag, (const UUID::uuid& tag_id), (override));

        MOCK_METHOD(void, delete_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(core::search_result<UUID::uuid>, find_posts, (
            const core::post_query& query
        ), (override));

        MOCK_METHOD(core::search_result<UUID::uuid>, find_tags, (
            const core::tag_query& query
        ), (override));

        MOCK_METHOD(void, remove_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(void, update_post_date_modified, (
            const UUID::uuid& post_id,
            std::string_view date_modified
        ), (override));

        MOCK_METHOD(void, update_post_description, (
            const core::post_update& post
        ), (override));

        MOCK_METHOD(void, update_post_title, (
            const core::post_update& post
        ), (override));

        MOCK_METHOD(void, update_tag_name, (
            const UUID::uuid& tag_id,
            std::string_view old_name,
            std::string_view new_name
        ), (override));
    };
}
