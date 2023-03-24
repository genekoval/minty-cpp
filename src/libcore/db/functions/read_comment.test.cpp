#include "database.test.hpp"

TEST_F(DatabasePostTest, ReadComment) {
    constexpr auto text = "A comment.";

    run([&]() -> ext::task<> {
        const auto post_id = co_await create_post();

        const auto comment = co_await db->create_comment(post_id, text);
        const auto result = co_await db->read_comment(comment.id);

        EXPECT_EQ(comment, result);
    }());
}
