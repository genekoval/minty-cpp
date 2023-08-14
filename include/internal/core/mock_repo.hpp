#include "repo.hpp"

#include <gmock/gmock.h>

namespace minty::core {
    struct mock_repo : repo {
        MOCK_METHOD(ext::task<comment_data>, add_comment, (
            const UUID::uuid& post_id,
            std::string_view content
        ), (override));

        MOCK_METHOD(ext::task<std::vector<object_preview>>, add_objects_url, (
            std::string_view url
        ), (override));

        MOCK_METHOD(ext::task<time_point>, add_post_objects, (
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            const std::optional<UUID::uuid>& destination
        ), (override));

        MOCK_METHOD(ext::task<>, add_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::task<>, add_related_post, (
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ), (override));

        MOCK_METHOD(ext::task<comment_data>, add_reply, (
            const UUID::uuid& parent_id,
            std::string_view content
        ), (override));

        MOCK_METHOD(ext::task<UUID::uuid>, add_tag, (
            std::string_view name
        ), (override));

        MOCK_METHOD(ext::task<tag_name>, add_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(ext::task<source>, add_tag_source, (
            const UUID::uuid& tag_id,
            std::string_view url
        ), (override));

        MOCK_METHOD(ext::task<>, create_indices, (), (override));

        MOCK_METHOD(ext::task<>, create_post, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(ext::task<UUID::uuid>, create_post_draft, (), (override));

        MOCK_METHOD(ext::task<>, delete_post, (
            const UUID::uuid& id
        ), (override));

        MOCK_METHOD(ext::task<time_point>, delete_post_objects, (
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects
        ), (override));

        MOCK_METHOD(ext::task<>, delete_post_tag, (
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ), (override));

        MOCK_METHOD(ext::task<>, delete_related_post, (
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ), (override));

        MOCK_METHOD(ext::task<>, delete_tag, (
            const UUID::uuid& id
        ), (override));

        MOCK_METHOD(ext::task<tag_name>, delete_tag_alias, (
            const UUID::uuid& tag_id,
            std::string_view alias
        ), (override));

        MOCK_METHOD(ext::task<>, delete_tag_source, (
            const UUID::uuid& tag_id,
            std::int64_t source_id
        ), (override));

        MOCK_METHOD(
            const UUID::uuid&,
            get_bucket_id,
            (),
            (const, noexcept, override)
        );

        MOCK_METHOD(ext::task<comment>, get_comment, (
            const UUID::uuid& comment_id
        ), (override));

        MOCK_METHOD(ext::task<comment_tree>, get_comments, (
            const UUID::uuid& post_id
        ), (override));

        MOCK_METHOD(ext::task<object>, get_object, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(
            ext::task<std::vector<object_error>>,
            get_object_preview_errors,
            (),
            (override)
        );

        MOCK_METHOD(ext::task<post>, get_post, (
            const UUID::uuid& id
        ), (override));

        MOCK_METHOD(ext::task<search_result<post_preview>>, get_posts, (
            const post_query& query
        ), (override));

        MOCK_METHOD(ext::task<tag>, get_tag, (
            const UUID::uuid& id
        ), (override));

        MOCK_METHOD(ext::task<search_result<tag_preview>>, get_tags, (
            const tag_query& query
        ), (override));

        MOCK_METHOD(ext::task<>, prune, (), (override));

        MOCK_METHOD(ext::task<std::optional<UUID::uuid>>, regenerate_preview, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(ext::task<std::size_t>, regenerate_previews, (
            unsigned int batch_size,
            unsigned int jobs,
            progress& progress
        ), (override));

        MOCK_METHOD(ext::task<std::string>, set_comment_content, (
            const UUID::uuid& comment_id,
            std::string_view content
        ), (override));

        MOCK_METHOD(
            ext::task<modification<std::optional<std::string>>>,
            set_post_description,
            (const UUID::uuid& post_id, std::string_view description),
            (override)
        );

        MOCK_METHOD(
            ext::task<modification<std::optional<std::string>>>,
            set_post_title,
            (const UUID::uuid& post_id, std::string_view title),
            (override)
        );

        MOCK_METHOD(
            ext::task<std::optional<std::string>>,
            set_tag_description,
            (const UUID::uuid& tag_id, std::string_view description),
            (override)
        );

        MOCK_METHOD(ext::task<tag_name>, set_tag_name, (
            const UUID::uuid& tag_id,
            std::string_view new_name
        ), (override));
    };
}
