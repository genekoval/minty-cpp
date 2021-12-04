#include "database.test.h"

TEST_F(DatabasePostTest, DeletePost) {
    const auto post1 = create_post();
    const auto post2 = create_post();

    ASSERT_EQ(2, count("post"));

    database.delete_post(post1);

    ASSERT_EQ(1, count("post"));

    const auto remaining = database.read_post(post2);
    ASSERT_EQ(post2, remaining.id);
}
