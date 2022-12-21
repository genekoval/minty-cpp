#include "database.test.hpp"

TEST_F(DatabasePostTest, DeleteRelatedPost) {
    const auto post = create_post();
    const auto related = create_post();

    database.create_related_post(post, related);
    database.delete_related_post(post, related);

    ASSERT_EQ(0, count("related_post"));
    ASSERT_EQ(2, count("post"));
}
