#include "database.test.hpp"

TEST_F(DatabasePostTest, CreateReply) {
    constexpr auto text = "Second";

    run([&]() -> ext::task<> {
        const auto post_id = co_await create_draft();
        const auto root = co_await db->create_comment(post_id, "First");
        const auto reply = co_await db->create_reply(root.id, text);

        EXPECT_EQ(post_id, reply.post_id);
        EXPECT_EQ(root.id, reply.parent_id);
        EXPECT_EQ(1, reply.indent);
        EXPECT_EQ(text, reply.content);
    }());
}
