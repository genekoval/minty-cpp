#include "database.test.hpp"

TEST_F(DatabasePostTest, DeleteRelatedPost) {
    run([&]() -> ext::task<> {
        const auto post = co_await create_post();
        const auto related = co_await create_post();

        co_await db->create_related_post(post, related);
        co_await db->delete_related_post(post, related);

        EXPECT_EQ(0, co_await count("related_post"));
        EXPECT_EQ(2, co_await count("post"));
    }());
}
