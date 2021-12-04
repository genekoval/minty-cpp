#include "database.test.h"

TEST_F(DatabasePostTest, UpdatePostTitleDateModified) {
    const auto id = create_post();
    database.update_post_title(id, "New Title");
    const auto post = database.read_post(id);

    ASSERT_NE(post.date_created, post.date_modified);
}
