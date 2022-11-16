#include "search.test.h"

TEST_F(SearchPostTest, AddPostTag) {
    const auto& post = add_post();

    search.add_post_tag(post.id, tag);

    const auto tags = get_post(post.id)["tags"].get<std::vector<UUID::uuid>>();

    ASSERT_EQ(post.tags.size() + 1, tags.size());

    for (auto i = 0ul; i < post.tags.size(); ++i) {
        EXPECT_EQ(post.tags.at(i), tags.at(i)) << "at index " << i;
    }

    EXPECT_EQ(tag, tags.back());
}
