#include "database.test.hpp"

TEST_F(DatabasePostTest, ReadRelatedPosts) {
    run([&]() -> ext::task<> {
        const auto post = co_await create_draft();
        const auto related = co_await create_draft();

        co_await db->create_related_post(post, related);

        const auto posts = co_await db->read_related_posts(post);

        EXPECT_EQ(1, posts.size());
        EXPECT_EQ(related, posts.at(0).id);
    }());
}
