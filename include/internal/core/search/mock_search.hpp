#pragma once

#include "search.hpp"

#include <gmock/gmock.h>

namespace minty::test {
    struct search_engine : core::search_engine {
        MOCK_METHOD(ext::task<>, add_post, (
            const core::db::post_search& post
        ), (override));

        MOCK_METHOD(ext::task<>, add_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::task<std::vector<std::string>>, add_posts, (
            std::span<const core::db::post_search> posts
        ), (override));

        MOCK_METHOD(ext::task<std::vector<std::string>>, add_tags, (
            std::span<const core::db::tag_search> tags
        ), (override));

        MOCK_METHOD(ext::task<>, add_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(ext::task<>, create_indices, (), (override));

        MOCK_METHOD(ext::task<>, delete_indices, (), (override));

        MOCK_METHOD(ext::task<>, delete_post, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(ext::task<>, delete_tag, (const UUID::uuid& tag_id), (override));

        MOCK_METHOD(ext::task<>, delete_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(ext::task<search_result<UUID::uuid>>, find_posts, (
            const post_query& query
        ), (override));

        MOCK_METHOD(ext::task<search_result<UUID::uuid>>, find_tags, (
            const tag_query& query
        ), (override));

        MOCK_METHOD(ext::task<>, remove_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::task<>, update_post_date_modified, (
            const UUID::uuid& post_id,
            time_point date_modified
        ), (override));

        MOCK_METHOD(ext::task<>, update_post_description, (
            const core::db::post_update& post
        ), (override));

        MOCK_METHOD(ext::task<>, update_post_title, (
            const core::db::post_update& post
        ), (override));

        MOCK_METHOD(ext::task<>, update_tag_name, (
            const UUID::uuid& tag_id,
            std::string_view old_name,
            std::string_view new_name
        ), (override));
    };
}
