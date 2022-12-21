#include "database.test.hpp"

TEST_F(DatabasePostObjectTest, MovePostObjectsOneToBeginning) {
    database.move_post_objects(post_id, {objects.back()}, objects.front());

    const auto result = with_sequence();

    ASSERT_EQ(3, result.size());

    ASSERT_EQ(objects.back(), result[0].id);
    ASSERT_EQ(objects.front(), result[1].id);
    ASSERT_EQ(objects[1], result[2].id);

    ASSERT_EQ(1, result[0].sequence);
    ASSERT_EQ(2, result[1].sequence);
    ASSERT_EQ(3, result[2].sequence);
}

TEST_F(DatabasePostObjectTest, MovePostObjectsOneToEnd) {
    database.move_post_objects(post_id, {objects.front()}, {});

    const auto result = with_sequence();

    ASSERT_EQ(3, result.size());

    ASSERT_EQ(objects.front(), result.back().id);
    ASSERT_EQ(objects[1], result.front().id);
    ASSERT_EQ(objects.back(), result[1].id);

    ASSERT_EQ(1, result[0].sequence);
    ASSERT_EQ(2, result[1].sequence);
    ASSERT_EQ(3, result[2].sequence);
}

TEST_F(DatabasePostObjectTest, MovePostObjectsMultiple) {
    database.move_post_objects(
        post_id,
        {objects[1], objects[2]},
        objects.front()
    );

    const auto result = with_sequence();

    ASSERT_EQ(3, result.size());

    ASSERT_EQ(objects[1], result.front().id);
    ASSERT_EQ(objects[2], result[1].id);
    ASSERT_EQ(objects[0], result[2].id);

    ASSERT_EQ(1, result[0].sequence);
    ASSERT_EQ(2, result[1].sequence);
    ASSERT_EQ(3, result[2].sequence);
}

TEST_F(DatabasePostObjectTest, MovePostObjectsDateModified) {
    const auto date_modified = database.move_post_objects(
        post_id,
        {objects.back()},
        objects.front()
    );
    const auto post = database.read_post(post_id);

    ASSERT_EQ(date_modified, post.date_modified);
    ASSERT_NE(post.date_created, post.date_modified);
}
