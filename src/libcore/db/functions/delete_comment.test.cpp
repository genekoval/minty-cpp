#include "database.test.hpp"

TEST_F(DatabasePostTest, DeleteLeafComment) {
    run([&]() -> ext::task<> {
        const auto post_id = co_await create_draft();

        const auto c1 = co_await db->create_comment(post_id, "Comment 1");
        const auto c2 = co_await db->create_comment(post_id, "Comment 2");

        EXPECT_TRUE(co_await db->delete_comment(c1.id, false));
        EXPECT_FALSE(co_await db->read_comment(c1.id));
        EXPECT_TRUE(co_await db->read_comment(c2.id));
        EXPECT_FALSE(co_await db->delete_comment(c1.id, false));
    }());
}

TEST_F(DatabasePostTest, DeleteCommentTree) {
    run([&]() -> ext::task<> {
        const auto post_id = co_await create_draft();

        const auto root1 = co_await db->create_comment(post_id, "Root 1");
        const auto root2 = co_await db->create_comment(post_id, "Root 2");

        const auto leaf = co_await db->create_reply(root1.id, "Leaf");

        EXPECT_TRUE(co_await db->delete_comment(root1.id, true));
        EXPECT_FALSE(co_await db->read_comment(root1.id));
        EXPECT_FALSE(co_await db->read_comment(leaf.id));
        EXPECT_TRUE(co_await db->read_comment(root2.id));
        EXPECT_FALSE(co_await db->delete_comment(root1.id, true));
    }());
}

TEST_F(DatabasePostTest, PruneComments) {
    run([&]() -> ext::task<> {
        const auto post_id = co_await create_draft();

        const auto c1 = co_await db->create_comment(post_id, "Comment 1");
        const auto c2 = co_await db->create_reply(c1.id, "Comment 2");

        EXPECT_TRUE(co_await db->delete_comment(c1.id, false));
        EXPECT_TRUE((co_await db->read_comment(c1.id)).value().content.empty());
        EXPECT_TRUE(co_await db->delete_comment(c2.id, false));
        EXPECT_FALSE(co_await db->read_comment(c1.id));
        EXPECT_FALSE(co_await db->read_comment(c2.id));
    }());
}
