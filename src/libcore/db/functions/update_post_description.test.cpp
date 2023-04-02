#include "database.test.hpp"

TEST_F(DatabasePostTest, UpdatePostDescriptionDateModified) {
    run([&]() -> ext::task<> {
        const auto id = co_await create_draft();
        co_await db->update_post_description(id, "New description.");
        const auto post = co_await db->read_post(id);

        EXPECT_NE(post.date_created, post.date_modified);
    }());
}
