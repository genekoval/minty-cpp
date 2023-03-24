#include "database.test.hpp"

TEST_F(DatabasePostTest, ReadZeroComments) {
    run([&]() -> ext::task<> {
        const auto post_id = co_await create_post();
        const auto comments = co_await db->read_comments(post_id);

        EXPECT_TRUE(comments.empty());
    }());
}

TEST_F(DatabasePostTest, ReadOneComment) {
    constexpr auto text = "First post.";

    run([&]() -> ext::task<> {
        const auto post_id = co_await create_post();
        const auto comment = co_await db->create_comment(post_id, text);

        auto comments = co_await db->read_comments(post_id);

        EXPECT_EQ(1, comments.size());
        EXPECT_EQ(comment, comments.at(0));
    }());
}

TEST_F(DatabasePostTest, ReadNestedComments) {
    run([&]() -> ext::task<> {
        const auto post_id = co_await create_post();

        const auto root1 = co_await db->create_comment(post_id, "Root 1");
        const auto child1 = co_await db->create_reply(root1.id, "Child 1");

        const auto root2 = co_await db->create_comment(post_id, "Root 2");
        const auto child2 = co_await db->create_reply(root2.id, "Child 2");

        const auto comments = co_await db->read_comments(post_id);

        EXPECT_EQ(4, comments.size());

        EXPECT_EQ(root2, comments.at(0));
        EXPECT_EQ(root1, comments.at(1));

        EXPECT_TRUE(comments.at(2) == child1 || comments.at(2) == child2);
        EXPECT_TRUE(comments.at(3) == child1 || comments.at(3) == child2);
    }());
}
