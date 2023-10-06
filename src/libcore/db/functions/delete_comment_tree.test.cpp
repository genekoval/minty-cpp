#include "database.test.hpp"

TEST_F(DatabasePostTest, DeleteCommentTree) {
    run([&]() -> ext::task<> {
        const auto post_id = co_await create_draft();

        const auto root1 = co_await db->create_comment(post_id, "Root 1");
        const auto root2 = co_await db->create_comment(post_id, "Root 2");

        const auto child1 = co_await db->create_reply(root1.id, "Child 1");
        const auto child2 = co_await db->create_reply(root2.id, "Child 2");

        EXPECT_TRUE(co_await db->delete_comment_tree(root1.id));
        EXPECT_FALSE(co_await db->read_comment(root1.id));
        EXPECT_FALSE(co_await db->read_comment(child1.id));
        EXPECT_TRUE(co_await db->read_comment(root2.id));
        EXPECT_TRUE(co_await db->read_comment(child2.id));
        EXPECT_FALSE(co_await db->delete_comment_tree(root1.id));

        EXPECT_TRUE(co_await db->delete_comment_tree(child2.id));
        EXPECT_FALSE(co_await db->read_comment(child2.id));
        EXPECT_TRUE(co_await db->read_comment(root2.id));
        EXPECT_FALSE(co_await db->delete_comment_tree(child2.id));

        EXPECT_TRUE(co_await db->delete_comment_tree(root2.id));
        EXPECT_FALSE(co_await db->read_comment(root2.id));
        EXPECT_FALSE(co_await db->delete_comment_tree(root2.id));
    }());
}
