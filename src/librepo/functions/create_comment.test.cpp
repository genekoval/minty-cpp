#include "database.test.h"

TEST_F(DatabasePostTest, CreateRootComment) {
    constexpr auto content = "First comment.";

    const auto post_id = create_post();
    const auto comment = database.create_comment(post_id, {}, content);

    ASSERT_EQ(post_id, comment.post_id);
    ASSERT_FALSE(comment.parent_id.has_value());
    ASSERT_EQ(0, comment.indent);
    ASSERT_EQ(content, comment.content);
}

TEST_F(DatabasePostTest, CreateChildComment) {
    constexpr auto text = "Child comment.";

    const auto post_id = create_post();
    const auto root = database.create_comment(post_id, {}, "First comment.");
    const auto child = database.create_comment(post_id, root.id, text);

    ASSERT_EQ(post_id, child.post_id);
    ASSERT_EQ(root.id, child.parent_id);
    ASSERT_EQ(0, root.indent);
    ASSERT_EQ(1, child.indent);
    ASSERT_EQ(text, child.content);
}
