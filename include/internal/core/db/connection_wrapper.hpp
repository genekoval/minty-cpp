#pragma once

#include "connection.hpp"

namespace minty::core::db {
    class connection_wrapper : connection {
        std::shared_ptr<connection> inner;
    public:
        explicit connection_wrapper(std::shared_ptr<connection>&& inner);

        auto begin() -> ext::task<pg::transaction> override;

        auto create_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) -> ext::task<comment> override;

        auto create_object(
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id,
            const std::optional<std::int64_t>& source_id
        ) -> ext::task<> override;

        auto create_object_preview_error(
            const UUID::uuid& object_id,
            std::string_view message
        ) -> ext::task<> override;

        auto create_post(
            std::string_view title,
            std::string_view description,
            const std::vector<UUID::uuid>& objects,
            const std::vector<UUID::uuid>& tags
        ) -> ext::task<post_search> override;

        auto create_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            std::int16_t position
        ) -> ext::task<time_point> override;

        auto create_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<> override;

        auto create_reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) -> ext::task<comment> override;

        auto create_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> ext::task<> override;

        auto create_site(
            std::string_view scheme,
            std::string_view name,
            std::optional<UUID::uuid> icon
        ) -> ext::task<site> override;

        auto create_source(
            std::int64_t site_id,
            std::string_view resource
        ) -> ext::task<source> override;

        auto create_tag(
            std::string_view name
        ) -> ext::task<UUID::uuid> override;

        auto create_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<tag_name> override;

        auto create_tag_source(
            const UUID::uuid& tag_id,
            std::int64_t source_id
        ) -> ext::task<> override;

        auto delete_object_preview_error(
            const UUID::uuid& object_id
        ) -> ext::task<> override;

        auto delete_post(const UUID::uuid& post_id) -> ext::task<> override;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects
        ) -> ext::task<decltype(post::date_modified)> override;

        auto delete_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<> override;

        auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> ext::task<> override;

        auto delete_tag(const UUID::uuid& tag_id) -> ext::task<> override;

        auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<tag_name> override;

        auto delete_tag_source(
            const UUID::uuid& tag_id,
            std::int64_t source_id
        ) -> ext::task<> override;

        auto move_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            const std::optional<UUID::uuid>& destination
        ) -> ext::task<time_point> override;

        auto prune() -> ext::task<> override;

        auto prune_objects() -> ext::task<std::vector<UUID::uuid>> override;

        auto read_comment(
            const UUID::uuid& comment_id
        ) -> ext::task<comment> override;

        auto read_comments(
            const UUID::uuid& post_id
        ) -> ext::task<std::vector<comment>> override;

        auto read_object(
            const UUID::uuid& object_id
        ) -> ext::task<object> override;

        auto read_object_posts(
            const UUID::uuid& object_id
        ) -> ext::task<std::vector<post_preview>> override;

        auto read_object_preview_errors() ->
            ext::task<std::vector<object_error>> override;

        auto read_objects(
            int batch_size
        ) -> ext::task<pg::portal<object_preview>> override;

        auto read_post(const UUID::uuid& post_id) -> ext::task<post> override;

        auto read_posts(
            const std::vector<UUID::uuid>& posts
        ) -> ext::task<std::vector<post_preview>> override;

        auto read_post_objects(
            const UUID::uuid& post_id
        ) -> ext::task<std::vector<object_preview>> override;

        auto read_post_search(
            int batch_size
        ) -> ext::task<pg::portal<post_search>> override;

        auto read_post_tags(
            const UUID::uuid& post_id
        ) -> ext::task<std::vector<tag_preview>> override;

        auto read_related_posts(
            const UUID::uuid& post_id
        ) -> ext::task<std::vector<post_preview>> override;

        auto read_site(
            std::string_view scheme,
            std::string_view host
        ) -> ext::task<std::optional<std::int64_t>> override;

        auto read_tag(const UUID::uuid& tag_id) -> ext::task<tag> override;

        auto read_tag_previews(
            const std::vector<UUID::uuid>& tags
        ) -> ext::task<std::vector<tag_preview>> override;

        auto read_tag_sources(
            const UUID::uuid& tag_id
        ) -> ext::task<std::vector<source>> override;

        auto read_tag_search(
            int batch_size
        ) -> ext::task<pg::portal<tag_search>> override;

        auto read_total_objects() -> ext::task<std::size_t> override;

        auto update_comment(
            const UUID::uuid& comment_id,
            std::string_view content
        ) -> ext::task<> override;

        auto update_object_preview(
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id
        ) -> ext::task<> override;

        auto update_post_description(
            const UUID::uuid& post_id,
            std::string_view description
        ) -> ext::task<post_update> override;

        auto update_post_title(
            const UUID::uuid& post_id,
            std::string_view title
        ) -> ext::task<post_update> override;

        auto update_tag_description(
            const UUID::uuid& tag_id,
            std::string_view description
        ) -> ext::task<std::optional<std::string>> override;

        auto update_tag_name(
            const UUID::uuid& tag_id,
            std::string_view name
        ) -> ext::task<tag_name_update> override;
    };
}
