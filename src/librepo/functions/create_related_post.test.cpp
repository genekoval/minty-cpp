#include "database.test.h"

namespace {
    struct related_post {
        std::string post_id;
        std::string related;
    };
}

namespace entix {
    ENTITY(related_post);

    READ_ENTITY(
        related_post,
        &related_post::post_id,
        &related_post::related
    )
}

TEST_F(DatabasePostTest, CreateRelatedPost) {
    const auto post = create_post();
    const auto related = create_post();

    database.create_related_post(post, related);

    ASSERT_EQ(1, count("related_post"));

    const auto result = exec1<related_post>(
        "SELECT post_id, related FROM data.related_post"
    );

    ASSERT_EQ(post, result.post_id);
    ASSERT_EQ(related, result.related);
}

TEST_F(DatabasePostTest, CreateRelatedPostIdempotent) {
    const auto post = create_post();
    const auto related = create_post();

    for (auto i = 0; i < 2; ++i) database.create_related_post(post, related);

    ASSERT_EQ(1, count("related_post"));
}
