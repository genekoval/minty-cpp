#include "database.test.hpp"

TEST_F(DatabasePostTest, ReadRelatedPosts) {
    const auto post = create_post();
    const auto related = create_post();

    database.create_related_post(post, related);

    const auto posts = database.read_related_posts(post);

    ASSERT_EQ(1, posts.size());
    ASSERT_EQ(related, posts.front().id);
}
