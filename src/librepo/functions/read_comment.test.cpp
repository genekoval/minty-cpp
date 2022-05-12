#include "database.test.h"

TEST_F(DatabasePostTest, ReadComment) {
    constexpr auto text = "A comment.";

    const auto post_id = create_post();

    const auto comment = database.create_comment(post_id, text);
    const auto result = database.read_comment(comment.id);

    ASSERT_EQ(comment, result);
}
