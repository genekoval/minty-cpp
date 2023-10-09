#include "database.test.hpp"

TEST_F(DatabasePostTest, UpdatePostDescription) {
    run([&]() -> ext::task<> {
        constexpr auto description = "New description.";

        const auto id = co_await create_draft();
        const auto modified =
            (co_await db->update_post_description(id, description)).value();
        const auto post = (co_await db->read_post(id)).value();

        EXPECT_EQ(description, post.description);
        EXPECT_EQ(modified, post.date_modified);
        EXPECT_NE(post.date_created, post.date_modified);
    }());
}
