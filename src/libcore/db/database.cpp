#include <internal/core/db/database.hpp>

namespace minty::core::db {
    database::database(const pg::parameters& params) : pool(provider(params)) {}

#ifdef TEST
    auto database::connect() -> ext::task<connection_wrapper> {
        co_return connection_wrapper(std::shared_ptr<connection>(
            new connection(co_await pool.checkout())
        ));
    }
#else
    auto database::connect() -> ext::task<connection> {
        co_return connection(co_await pool.checkout());
    }
#endif

    database::provider::provider(const pg::parameters& params) :
        params(params)
    {}

    auto database::provider::provide() -> ext::task<pg::client> {
        auto client = co_await pg::connect(params);

        co_await client.prepare_fn(
            "create_comment",
            &connection::create_comment
        );
        co_await client.prepare_fn(
            "create_object",
            &connection::create_object
        );
        co_await client.prepare_fn(
            "create_object_preview_error",
            &connection::create_object_preview_error
        );
        co_await client.prepare_fn(
            "create_post",
            &connection::create_post
        );
        co_await client.prepare_fn(
            "create_post_draft",
            &connection::create_post_draft
        );
        co_await client.prepare_fn(
            "create_post_objects",
            &connection::create_post_objects
        );
        co_await client.prepare_fn(
            "create_post_tag",
            &connection::create_post_tag
        );
        co_await client.prepare_fn(
            "create_reply",
            &connection::create_reply
        );
        co_await client.prepare_fn(
            "create_related_post",
            &connection::create_related_post
        );
        co_await client.prepare_fn(
            "create_site",
            &connection::create_site
        );
        co_await client.prepare_fn(
            "create_source",
            &connection::create_source
        );
        co_await client.prepare_fn(
            "create_tag",
            &connection::create_tag
        );
        co_await client.prepare_fn(
            "create_tag_alias",
            &connection::create_tag_alias
        );
        co_await client.prepare_fn(
            "create_tag_source",
            &connection::create_tag_source
        );
        co_await client.prepare_fn(
            "delete_object_preview_error",
            &connection::delete_object_preview_error
        );
        co_await client.prepare_fn(
            "delete_post",
            &connection::delete_post
        );
        co_await client.prepare_fn(
            "delete_post_objects",
            &connection::delete_post_objects
        );
        co_await client.prepare_fn(
            "delete_post_tag",
            &connection::delete_post_tag
        );
        co_await client.prepare_fn(
            "delete_related_post",
            &connection::delete_related_post
        );
        co_await client.prepare_fn(
            "delete_tag",
            &connection::delete_tag
        );
        co_await client.prepare_fn(
            "delete_tag_alias",
            &connection::delete_tag_alias
        );
        co_await client.prepare_fn(
            "delete_tag_source",
            &connection::delete_tag_source
        );
        co_await client.prepare_fn(
            "move_post_objects",
            &connection::move_post_objects
        );
        co_await client.prepare_fn(
            "prune",
            &connection::prune
        );
        co_await client.prepare_fn(
            "prune_objects",
            &connection::prune_objects
        );
        co_await client.prepare_fn(
            "read_comment",
            &connection::read_comment
        );
        co_await client.prepare_fn(
            "read_comments",
            &connection::read_comments
        );
        co_await client.prepare_fn(
            "read_object",
            &connection::read_object
        );
        co_await client.prepare_fn(
            "read_object_posts",
            &connection::read_object_posts
        );
        co_await client.prepare_fn(
            "read_object_preview_errors",
            &connection::read_object_preview_errors
        );
        co_await client.prepare("read_objects", "SELECT * FROM read_objects()");
        co_await client.prepare_fn(
            "read_post",
            &connection::read_post
        );
        co_await client.prepare_fn(
            "read_posts",
            &connection::read_posts
        );
        co_await client.prepare_fn(
            "read_post_objects",
            &connection::read_post_objects
        );
        co_await client.prepare(
            "read_post_search",
            "SELECT * FROM read_post_search()"
        );
        co_await client.prepare_fn(
            "read_post_tags",
            &connection::read_post_tags
        );
        co_await client.prepare_fn(
            "read_related_posts",
            &connection::read_related_posts
        );
        co_await client.prepare_fn(
            "read_site",
            &connection::read_site
        );
        co_await client.prepare_fn(
            "read_tag",
            &connection::read_tag
        );
        co_await client.prepare_fn(
            "read_tag_previews",
            &connection::read_tag_previews
        );
        co_await client.prepare_fn(
            "read_tag_sources",
            &connection::read_tag_sources
        );
        co_await client.prepare(
            "read_tag_search",
            "SELECT * FROM read_tag_search()"
        );
        co_await client.prepare_fn(
            "read_total_objects",
            &connection::read_total_objects
        );
        co_await client.prepare_fn(
            "update_comment",
            &connection::update_comment
        );
        co_await client.prepare_fn(
            "update_object_preview",
            &connection::update_object_preview
        );
        co_await client.prepare_fn(
            "update_post_description",
            &connection::update_post_description
        );
        co_await client.prepare_fn(
            "update_post_title",
            &connection::update_post_title
        );
        co_await client.prepare_fn(
            "update_tag_description",
            &connection::update_tag_description
        );
        co_await client.prepare_fn(
            "update_tag_name",
            &connection::update_tag_name
        );

        co_return client;
    }
}
