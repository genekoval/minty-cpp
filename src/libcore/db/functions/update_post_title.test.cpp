#include "database.test.hpp"

TEST_F(DatabasePostTest, UpdatePostTitleDateModified) {
    run([&]() -> ext::task<> {
        const auto id = co_await create_post();
        co_await db->update_post_title(id, "New Title");
        const auto post = co_await db->read_post(id);

        EXPECT_NE(post.date_created, post.date_modified);
    }());
}
