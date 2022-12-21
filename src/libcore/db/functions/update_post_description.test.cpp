#include "database.test.hpp"

TEST_F(DatabasePostTest, UpdatePostDescriptionDateModified) {
    const auto id = create_post();

    database.update_post_description(id, "New description.");

    const auto post = database.read_post(id);

    ASSERT_NE(post.date_created, post.date_modified);
}
