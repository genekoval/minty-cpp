#include "DatabasePost.test.hpp"

using minty::test::related_post;

PGCPP_COMPOSITE_DEFINE(
    related_post,
    &related_post::post_id,
    &related_post::related
);

auto DatabasePostTest::create_draft() -> ext::task<UUID::uuid> {
    const auto draft = co_await db->create_post_draft();
    co_return draft.id;
}

auto DatabasePostTest::read_related_posts()
    -> ext::task<std::vector<minty::test::related_post>> {
    co_return co_await client->fetch_rows<related_post>(
        "SELECT post_id, related FROM data.related_post"
    );
}

auto DatabasePostTest::tables() -> std::vector<std::string> { return {"post"}; }
