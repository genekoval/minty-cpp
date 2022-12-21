#include "database.test.hpp"

TEST_F(DatabasePostTest, CreateComment) {
    constexpr auto content = "First comment.";

    const auto post_id = create_post();
    const auto comment = database.create_comment(post_id, content);

    ASSERT_EQ(post_id, comment.post_id);
    ASSERT_FALSE(comment.parent_id.has_value());
    ASSERT_EQ(0, comment.indent);
    ASSERT_EQ(content, comment.content);
}
