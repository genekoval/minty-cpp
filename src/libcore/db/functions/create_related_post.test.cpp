#include "database.test.hpp"

TEST_F(DatabasePostTest, CreateRelatedPost) {

    run([&]() -> ext::task<> {
        const auto post = co_await create_post();
        const auto related = co_await create_post();

        co_await db->create_related_post(post, related);

        const auto related_posts = co_await read_related_posts();

        EXPECT_EQ(1, related_posts.size());

        const auto& result = related_posts.front();

        EXPECT_EQ(post, result.post_id);
        EXPECT_EQ(related, result.related);
    }());
}

TEST_F(DatabasePostTest, CreateRelatedPostIdempotent) {
    run([&]() -> ext::task<> {
        const auto post = co_await create_post();
        const auto related = co_await create_post();

        for (auto i = 0; i < 2; ++i) {
            co_await db->create_related_post(post, related);
        }

        EXPECT_EQ(1, co_await count("related_post"));
    }());
}
