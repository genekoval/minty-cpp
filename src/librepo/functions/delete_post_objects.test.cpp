#include "database.test.h"

TEST_F(DatabasePostObjectTest, DeletePostObjects) {
    database.delete_post_objects(post_id, {objects[1], objects[2]});

    const auto result = database.read_post_objects(post_id);

    ASSERT_EQ(1, result.size());
    ASSERT_EQ(objects.front(), result.front().id);
}

TEST_F(DatabasePostObjectTest, DeletePostObjectsDateModified) {
    const auto date_modified = database.delete_post_objects(
        post_id,
        {objects.front()}
    );
    const auto post = database.read_post(post_id);

    ASSERT_EQ(date_modified, post.date_modified);
    ASSERT_NE(post.date_created, post.date_modified);
}

TEST_F(DatabasePostObjectTest, DeletePostObjectsSequence) {
    database.delete_post_objects(post_id, {objects.front()});

    const auto result = with_sequence();

    ASSERT_EQ(objects[1], result[0].id);
    ASSERT_EQ(1, result[0].sequence);

    ASSERT_EQ(objects[2], result[1].id);
    ASSERT_EQ(2, result[1].sequence);
}
