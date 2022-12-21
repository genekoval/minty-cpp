#include "database.test.hpp"

TEST_F(DatabasePostTest, CreateReply) {
    constexpr auto text = "A reply comment.";

    const auto post_id = create_post();
    const auto root = database.create_comment(post_id, "First comment.");
    const auto reply = database.create_reply(root.id, text);

    ASSERT_EQ(post_id, reply.post_id);
    ASSERT_EQ(root.id, reply.parent_id);
    ASSERT_EQ(1, reply.indent);
    ASSERT_EQ(text, reply.content);
}
