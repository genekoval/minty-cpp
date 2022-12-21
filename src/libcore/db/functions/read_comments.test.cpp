#include "database.test.hpp"

TEST_F(DatabasePostTest, ReadZeroComments) {
    const auto post_id = create_post();
    const auto comments = database.read_comments(post_id);

    ASSERT_TRUE(comments.empty());
}

TEST_F(DatabasePostTest, ReadOneComment) {
    constexpr auto text = "First post.";

    const auto post_id = create_post();
    const auto comment = database.create_comment(post_id, text);

    auto comments = database.read_comments(post_id);

    ASSERT_EQ(1, comments.size());
    ASSERT_EQ(comment, comments.front());
}

TEST_F(DatabasePostTest, ReadNestedComments) {
    const auto post_id = create_post();

    const auto root1 = database.create_comment(post_id, "Root 1");
    const auto child1 = database.create_reply(root1.id, "Child 1");

    const auto root2 = database.create_comment(post_id, "Root 2");
    const auto child2 = database.create_reply(root2.id, "Child 2");

    const auto comments = database.read_comments(post_id);

    ASSERT_EQ(4, comments.size());

    ASSERT_EQ(root2, comments[0]);
    ASSERT_EQ(root1, comments[1]);

    ASSERT_TRUE(comments[2] == child1 || comments[2] == child2);
    ASSERT_TRUE(comments[3] == child1 || comments[3] == child2);
}
