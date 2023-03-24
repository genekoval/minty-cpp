#include "database.test.hpp"

TEST_F(DatabasePostTest, CreateComment) {
    constexpr auto content = "First comment.";

    run([&]() -> ext::task<> {
        const auto post_id = co_await create_post();
        const auto comment = co_await db->create_comment(post_id, content);

        EXPECT_EQ(post_id, comment.post_id);
        EXPECT_FALSE(comment.parent_id.has_value());
        EXPECT_EQ(0, comment.indent);
        EXPECT_EQ(content, comment.content);
    }());
}
