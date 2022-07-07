#include "search.test.h"

TEST_F(SearchPostTest, RemovePostTag) {
    const auto& post = add_post();

    search.remove_post_tag(post.id, post.tags.front());

    auto res = get_post(post.id);
    const auto tags = get_tags(res);

    ASSERT_EQ(post.tags.size() - 1, tags.size());

    for (auto i = 0ul; i < tags.size(); ++i) {
        EXPECT_EQ(post.tags.at(i + 1), tags.at(i)) << " at index " << i;
    }
}
