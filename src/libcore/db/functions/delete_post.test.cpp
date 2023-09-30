#include "database.test.hpp"

TEST_F(DatabasePostTest, DeletePost) {
    run([&]() -> ext::task<> {
        const auto post1 = co_await create_draft();
        const auto post2 = co_await create_draft();

        EXPECT_EQ(2, co_await count("post"));

        co_await db->delete_post(post1);

        EXPECT_EQ(1, co_await count("post"));

        const auto remaining = (co_await db->read_post(post2)).value();
        EXPECT_EQ(post2, remaining.id);
    }());
}
