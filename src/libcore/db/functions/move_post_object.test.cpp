#include "database.test.hpp"

TEST_F(DatabasePostObjectTest, MoveObject) {
    database.move_post_object(post_id, 2, 0);

    const auto result = with_sequence();

    ASSERT_EQ(3, result.size());

    ASSERT_EQ(objects[2], result[0].id);
    ASSERT_EQ(objects[0], result[1].id);
    ASSERT_EQ(objects[1], result[2].id);

    ASSERT_EQ(1, result[0].sequence);
    ASSERT_EQ(2, result[1].sequence);
    ASSERT_EQ(3, result[2].sequence);
}

TEST_F(DatabasePostObjectTest, MovePostObjectDateModified) {
    const auto date_modified = database.move_post_object(post_id, 0, 1);
    const auto post = database.read_post(post_id);

    ASSERT_EQ(date_modified, post.date_modified);
    ASSERT_NE(post.date_created, post.date_modified);
}
