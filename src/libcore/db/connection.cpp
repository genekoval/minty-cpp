#include <internal/core/db/connection.hpp>
#include <internal/core/db/model/comment.hpp>
#include <internal/core/db/model/object_error.hpp>
#include <internal/core/db/model/tag_name.hpp>
#include <internal/core/db/model/tag_preview.hpp>

#include <minty/except.hpp>

using pg::sqlstate;

namespace minty::core::db {
    connection::connection(ext::pool_item<pg::client>&& client) :
        client(std::forward<ext::pool_item<pg::client>>(client))
    {}

    auto connection::begin() -> ext::task<pg::transaction> {
        co_return co_await client->begin();
    }

    auto connection::create_comment(
        const UUID::uuid& post_id,
        std::string_view content
    ) -> ext::task<comment> {
        co_return co_await client->fetch_prepared<comment>(
            __FUNCTION__,
            post_id,
            content
        );
    }

    auto connection::create_object(
        const UUID::uuid& object_id,
        const std::optional<UUID::uuid>& preview_id,
        const std::optional<std::int64_t>& source_id
    ) -> ext::task<> {
        co_await client->query_prepared(
            __FUNCTION__,
            object_id,
            preview_id,
            source_id
        );
    }

    auto connection::create_object_preview_error(
        const UUID::uuid& object_id,
        std::string_view message
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, object_id, message);
    }

    auto connection::create_post(
        const UUID::uuid& post_id
    ) -> ext::task<time_point> {
        try {
            co_return co_await client->fetch_prepared<time_point>(
                __FUNCTION__,
                post_id
            );
        }
        catch (const pg::sql_error& error) {
            if (error.sqlstate(sqlstate::no_data_found)) {
                throw not_found(error.fields().message);
            }

            throw;
        }
    }

    auto connection::create_post_draft() -> ext::task<post_search> {
        const auto [id, created] = co_await client->fetch_prepared<
            std::tuple<UUID::uuid, time_point>
        >(__FUNCTION__);

        co_return post_search {
            .id = id,
            .visibility = visibility::draft,
            .created = created,
            .modified = created
        };
    }

    auto connection::create_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        const std::optional<UUID::uuid>& destination
    ) -> ext::task<time_point> {
        co_return co_await client->fetch_prepared<time_point>(
            __FUNCTION__,
            post_id,
            objects,
            destination
        );
    }

    auto connection::create_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, post_id, tag_id);
    }

    auto connection::create_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, post_id, related);
    }

    auto connection::create_reply(
        const UUID::uuid& parent_id,
        std::string_view content
    ) -> ext::task<comment> {
        co_return co_await client->fetch_prepared<comment>(
            __FUNCTION__,
            parent_id,
            content
        );
    }

    auto connection::create_site(
        std::string_view scheme,
        std::string_view name,
        std::optional<UUID::uuid> icon
    ) -> ext::task<site> {
        co_return co_await client->fetch_prepared<site>(
            __FUNCTION__,
            scheme,
            name,
            icon
        );
    }

    auto connection::create_source(
        std::int64_t site_id,
        std::string_view resource
    ) -> ext::task<source> {
        co_return co_await client->fetch_prepared<source>(
            __FUNCTION__,
            site_id,
            resource
        );
    }

    auto connection::create_tag(
        std::string_view name
    ) -> ext::task<UUID::uuid> {
        co_return co_await client->fetch_prepared<UUID::uuid>(
            __FUNCTION__,
            name
        );
    }

    auto connection::create_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        co_return co_await client->fetch_prepared<tag_name>(
            __FUNCTION__,
            tag_id,
            alias
        );
    }

    auto connection::create_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, tag_id, source_id);
    }

    auto connection::delete_object_preview_error(
        const UUID::uuid& object_id
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, object_id);
    }

    auto connection::delete_post(const UUID::uuid& post_id) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, post_id);
    }

    auto connection::delete_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects
    ) -> ext::task<time_point> {
        co_return co_await client->fetch_prepared<time_point>(
            __FUNCTION__,
            post_id,
            objects
        );
    }

    auto connection::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, post_id, tag_id);
    }

    auto connection::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, post_id, related);
    }

    auto connection::delete_tag(const UUID::uuid& tag_id) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, tag_id);
    }

    auto connection::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        co_return co_await client->fetch_prepared<tag_name>(
            __FUNCTION__,
            tag_id,
            alias
        );
    }

    auto connection::delete_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, tag_id, source_id);
    }

    auto connection::move_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        const std::optional<UUID::uuid>& destination
    ) -> ext::task<time_point> {
        co_return co_await client->fetch_prepared<time_point>(
            __FUNCTION__,
            post_id,
            objects,
            destination
        );
    }

    auto connection::prune() -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__);
    }

    auto connection::prune_objects() -> ext::task<std::vector<UUID::uuid>> {
        co_return co_await client->fetch_rows_prepared<UUID::uuid>(
            __FUNCTION__
        );
    }

    auto connection::read_comment(
        const UUID::uuid& comment_id
    ) -> ext::task<comment> {
        co_return co_await client->fetch_prepared<comment>(
            __FUNCTION__,
            comment_id
        );
    }

    auto connection::read_comments(
        const UUID::uuid& post_id
    ) -> ext::task<std::vector<comment>> {
        co_return co_await client->fetch_rows_prepared<comment>(
            __FUNCTION__,
            post_id
        );
    }

    auto connection::read_object(
        const UUID::uuid& object_id
    ) -> ext::task<object> {
        co_return co_await client->fetch_prepared<object>(
            __FUNCTION__,
            object_id
        );
    }

    auto connection::read_object_posts(
        const UUID::uuid& object_id
    ) -> ext::task<std::vector<post_preview>> {
        co_return co_await client->fetch_rows_prepared<post_preview>(
            __FUNCTION__,
            object_id
        );
    }

    auto connection::read_object_preview_errors() ->
        ext::task<std::vector<object_error>>
    {
        co_return co_await client->fetch_rows_prepared<object_error>(
            __FUNCTION__
        );
    }

    auto connection::read_objects(
        int batch_size
    ) -> ext::task<pg::portal<object_preview>> {
        co_return co_await client->stream_prepared<object_preview>(
            "",
            __FUNCTION__,
            batch_size
        );
    }

    auto connection::read_post(const UUID::uuid& post_id) -> ext::task<post> {
        co_return co_await client->fetch_prepared<post>(
            __FUNCTION__,
            post_id
        );
    }

    auto connection::read_posts(
        const std::vector<UUID::uuid>& posts
    ) -> ext::task<std::vector<post_preview>> {
        co_return co_await client->fetch_rows_prepared<post_preview>(
            __FUNCTION__,
            posts
        );
    }

    auto connection::read_post_objects(
        const UUID::uuid& post_id
    ) -> ext::task<std::vector<object_preview>> {
        co_return co_await client->fetch_rows_prepared<object_preview>(
            __FUNCTION__,
            post_id
        );
    }

    auto connection::read_post_search(
        int batch_size
    ) -> ext::task<pg::portal<post_search>> {
        co_return co_await client->stream_prepared<post_search>(
            "",
            __FUNCTION__,
            batch_size
        );
    }

    auto connection::read_post_tags(
        const UUID::uuid& post_id
    ) -> ext::task<std::vector<tag_preview>> {
        co_return co_await client->fetch_rows_prepared<tag_preview>(
            __FUNCTION__,
            post_id
        );
    }

    auto connection::read_related_posts(
        const UUID::uuid& post_id
    ) -> ext::task<std::vector<post_preview>> {
        co_return co_await client->fetch_rows_prepared<post_preview>(
            __FUNCTION__,
            post_id
        );
    }

    auto connection::read_site(
        std::string_view scheme,
        std::string_view host
    ) -> ext::task<std::optional<std::int64_t>> {
        co_return co_await client->fetch_prepared<std::optional<std::int64_t>>(
            __FUNCTION__,
            scheme,
            host
        );
    }

    auto connection::read_tag(const UUID::uuid& tag_id) -> ext::task<tag> {
        co_return co_await client->fetch_prepared<tag>(__FUNCTION__, tag_id);
    }

    auto connection::read_tag_previews(
        const std::vector<UUID::uuid>& tags
    ) -> ext::task<std::vector<tag_preview>> {
        co_return co_await client->fetch_rows_prepared<tag_preview>(
            __FUNCTION__,
            tags
        );
    }

    auto connection::read_tag_sources(
        const UUID::uuid& tag_id
    ) -> ext::task<std::vector<source>> {
        co_return co_await client->fetch_rows_prepared<source>(
            __FUNCTION__,
            tag_id
        );
    }

    auto connection::read_tag_search(
        int batch_size
    ) -> ext::task<pg::portal<tag_search>> {
        co_return co_await client->stream_prepared<tag_search>(
            "",
            __FUNCTION__,
            batch_size
        );
    }

    auto connection::read_total_objects() -> ext::task<std::size_t> {
        co_return co_await client->fetch_prepared<std::int64_t>(__FUNCTION__);
    }

    auto connection::update_comment(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, comment_id, content);
    }

    auto connection::update_object_preview(
        const UUID::uuid& object_id,
        const std::optional<UUID::uuid>& preview_id
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, object_id, preview_id);
    }

    auto connection::update_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> ext::task<post_update> {
        co_return co_await client->fetch_prepared<post_update>(
            __FUNCTION__,
            post_id,
            description
        );
    }

    auto connection::update_post_title(
        const UUID::uuid& post_id,
        std::string_view title
    ) -> ext::task<post_update> {
        co_return co_await client->fetch_prepared<post_update>(
            __FUNCTION__,
            post_id,
            title
        );
    }

    auto connection::update_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> ext::task<std::optional<std::string>> {
        co_return co_await client->fetch_prepared<std::optional<std::string>>(
            __FUNCTION__,
            tag_id,
            description
        );
    }

    auto connection::update_tag_name(
        const UUID::uuid& tag_id,
        std::string_view name
    ) -> ext::task<tag_name_update> {
        co_return co_await client->fetch_prepared<tag_name_update>(
            __FUNCTION__,
            tag_id,
            name
        );
    }
}
