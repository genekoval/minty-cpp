#include "database.test.h"

TEST_F(DatabasePostObjectTest, MoveObject) {
    const auto objects = std::vector<std::string> {
        "0e5bad49-9cf9-46e4-85c9-83ef9baf1f79",
        "adf85243-7673-467f-9fa6-56fc40642e06",
        "0797323f-910f-4f90-9bb1-dd145730c7a0",
        "fc6478d2-2acb-4773-82c9-fbd4f756303d",
        "edfd4b08-334a-4969-8fe7-0acc15a77045"
    };

    for (const auto& object : objects) {
        database.create_object(object, {}, {});
    }

    const auto post = database.create_post("", "", objects, {}).id;

    database.move_post_object(post, 3, 0);

    const auto result = database.read_post_objects(post);

    ASSERT_EQ(objects[3], result[0].id);
    ASSERT_EQ(objects[0], result[1].id);
    ASSERT_EQ(objects[1], result[2].id);
    ASSERT_EQ(objects[2], result[3].id);
    ASSERT_EQ(objects[4], result[4].id);
}

TEST_F(DatabasePostObjectTest, MovePostObjectDateModified) {
    const auto id = create_post_with_objects();

    database.move_post_object(id, 0, 1);

    const auto post = database.read_post(id);

    ASSERT_NE(post.date_created, post.date_modified);
}
