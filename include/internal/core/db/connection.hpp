#pragma once

#include "pool.hpp"

#include <internal/test.hpp>

#include <internal/core/db/model/object.hpp>
#include <internal/core/db/model/post.hpp>
#include <internal/core/db/model/post_preview.hpp>
#include <internal/core/db/model/post_search.hpp>
#include <internal/core/db/model/post_update.hpp>
#include <internal/core/db/model/site.hpp>
#include <internal/core/db/model/source.hpp>
#include <internal/core/db/model/tag.hpp>
#include <internal/core/db/model/tag_name_update.hpp>
#include <internal/core/db/model/tag_search.hpp>
#include <minty/model/comment.hpp>
#include <minty/model/object_error.hpp>
#include <minty/model/range.hpp>
#include <minty/model/tag_name.hpp>
#include <minty/model/tag_preview.hpp>

namespace minty::core::db {
    class connection {
        pool::item client;
    public:
        connection() = default;

        connection(ext::pool_item<pg::client>&& client);

        connection(const connection&) = delete;

        connection(connection&&) = default;

        VIRTUAL_DESTRUCTOR(connection);

        auto operator=(const connection&) -> connection& = delete;

        auto operator=(connection&&) -> connection& = default;

        VIRTUAL auto begin() -> ext::task<pg::transaction>;

        VIRTUAL auto create_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) -> ext::task<comment>;

        VIRTUAL auto create_object(
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id,
            const std::optional<std::int64_t>& source_id
        ) -> ext::task<>;

        VIRTUAL auto create_object_preview_error(
            const UUID::uuid& object_id,
            std::string_view message
        ) -> ext::task<>;

        VIRTUAL auto create_post(const UUID::uuid& post_id)
            -> ext::task<time_point>;

        VIRTUAL auto create_post_draft() -> ext::task<post_search>;

        VIRTUAL auto create_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            const std::optional<UUID::uuid>& destination
        ) -> ext::task<time_point>;

        VIRTUAL auto create_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        VIRTUAL auto create_reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) -> ext::task<comment>;

        VIRTUAL auto create_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> ext::task<>;

        VIRTUAL auto create_site(
            std::string_view scheme,
            std::string_view name,
            std::optional<UUID::uuid> icon
        ) -> ext::task<site>;

        VIRTUAL auto create_source(
            std::int64_t site_id,
            std::string_view resource
        ) -> ext::task<source>;

        VIRTUAL auto create_tag(std::string_view name) -> ext::task<UUID::uuid>;

        VIRTUAL auto create_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<tag_name>;

        VIRTUAL auto create_tag_source(
            const UUID::uuid& tag_id,
            std::int64_t source_id
        ) -> ext::task<>;

        VIRTUAL auto delete_comment(
            const UUID::uuid& comment_id,
            bool recursive
        ) -> ext::task<bool>;

        VIRTUAL auto delete_object_preview_error(const UUID::uuid& object_id)
            -> ext::task<>;

        VIRTUAL auto delete_post(const UUID::uuid& post_id) -> ext::task<>;

        VIRTUAL auto delete_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects
        ) -> ext::task<decltype(post::date_modified)>;

        VIRTUAL auto delete_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        VIRTUAL auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> ext::task<>;

        VIRTUAL auto delete_tag(const UUID::uuid& tag_id) -> ext::task<>;

        VIRTUAL auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<tag_name>;

        VIRTUAL auto delete_tag_source(
            const UUID::uuid& tag_id,
            std::int64_t source_id
        ) -> ext::task<>;

        VIRTUAL auto prune() -> ext::task<>;

        VIRTUAL auto prune_objects() -> ext::task<std::vector<UUID::uuid>>;

        VIRTUAL auto read_comment(const UUID::uuid& comment_id)
            -> ext::task<std::optional<comment>>;

        VIRTUAL auto read_comments(const UUID::uuid& post_id)
            -> ext::task<std::vector<comment>>;

        VIRTUAL auto read_object(const UUID::uuid& object_id)
            -> ext::task<std::optional<object>>;

        VIRTUAL auto read_object_posts(const UUID::uuid& object_id)
            -> ext::task<std::vector<post_preview>>;

        VIRTUAL auto read_object_preview_errors()
            -> ext::task<std::vector<object_error>>;

        VIRTUAL auto read_objects(int batch_size)
            -> ext::task<pg::portal<object_preview>>;

        VIRTUAL auto read_post(const UUID::uuid& post_id)
            -> ext::task<std::optional<post>>;

        VIRTUAL auto read_posts(const std::vector<UUID::uuid>& posts)
            -> ext::task<std::vector<post_preview>>;

        VIRTUAL auto read_post_search(int batch_size)
            -> ext::task<pg::portal<post_search>>;

        VIRTUAL auto read_post_tags(const UUID::uuid& post_id)
            -> ext::task<std::vector<tag_preview>>;

        VIRTUAL auto read_related_posts(const UUID::uuid& post_id)
            -> ext::task<std::vector<post_preview>>;

        VIRTUAL auto read_site(std::string_view scheme, std::string_view host)
            -> ext::task<std::optional<std::int64_t>>;

        VIRTUAL auto read_tag(const UUID::uuid& tag_id)
            -> ext::task<std::optional<tag>>;

        VIRTUAL auto read_tag_previews(const std::vector<UUID::uuid>& tags)
            -> ext::task<std::vector<tag_preview>>;

        VIRTUAL auto read_tag_sources(const UUID::uuid& tag_id)
            -> ext::task<std::vector<source>>;

        VIRTUAL auto read_tag_search(int batch_size)
            -> ext::task<pg::portal<tag_search>>;

        VIRTUAL auto read_total_objects() -> ext::task<std::size_t>;

        VIRTUAL auto update_comment(
            const UUID::uuid& comment_id,
            std::string_view content
        ) -> ext::task<>;

        VIRTUAL auto update_object_preview(
            const UUID::uuid& object_id,
            const std::optional<UUID::uuid>& preview_id
        ) -> ext::task<>;

        VIRTUAL auto update_post_description(
            const UUID::uuid& post_id,
            std::string_view description
        ) -> ext::task<post_update>;

        VIRTUAL auto update_post_title(
            const UUID::uuid& post_id,
            std::string_view title
        ) -> ext::task<post_update>;

        VIRTUAL auto update_tag_description(
            const UUID::uuid& tag_id,
            std::string_view description
        ) -> ext::task<std::optional<std::string>>;

        VIRTUAL auto update_tag_name(
            const UUID::uuid& tag_id,
            std::string_view name
        ) -> ext::task<tag_name_update>;
    };
}
