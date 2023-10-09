#include "database.test.hpp"

TEST_F(DatabasePostTest, UpdatePostTitle) {
    run([&]() -> ext::task<> {
        constexpr auto title = "New Title";

        const auto id = co_await create_draft();
        const auto modified =
            (co_await db->update_post_title(id, title)).value();
        const auto post = (co_await db->read_post(id)).value();

        EXPECT_EQ(title, post.title);
        EXPECT_EQ(modified, post.date_modified);
        EXPECT_NE(post.date_created, post.date_modified);
    }());
}
