#include "database.test.h"

TEST_F(DatabasePostObjectTest, DeletePostObjectsDateModified) {
    const auto id = create_post_with_objects();

    database.delete_post_objects(id, {objects.front()});

    const auto post = database.read_post(id);

    ASSERT_NE(post.date_created, post.date_modified);
}
