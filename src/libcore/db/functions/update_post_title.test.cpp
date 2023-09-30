#include "database.test.hpp"

TEST_F(DatabasePostTest, UpdatePostTitleDateModified) {
    run([&]() -> ext::task<> {
        const auto id = co_await create_draft();
        co_await db->update_post_title(id, "New Title");
        const auto post = (co_await db->read_post(id)).value();

        EXPECT_NE(post.date_created, post.date_modified);
    }());
}
