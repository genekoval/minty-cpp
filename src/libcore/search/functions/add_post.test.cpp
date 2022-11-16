#include "search.test.h"

TEST_F(SearchPostTest, AddPost) {
    const auto& post = add_post();
    auto res = get_post(post.id);

    const auto title = res["title"].get<std::string>();
    const auto description = res["description"].get<std::string>();

    const auto created = res["created"].get<minty::test::time_point>();
    const auto modified = res["modified"].get<minty::test::time_point>();

    auto tags = std::vector<UUID::uuid>();
    for (const auto& tag : res["tags"]) {
        tags.push_back(tag.get<UUID::uuid>());
    }

    ASSERT_EQ(post.title, title);
    ASSERT_EQ(post.description, description);

    ASSERT_EQ(post.created, created);
    ASSERT_EQ(post.modified, modified);

    ASSERT_EQ(post.tags.size(), tags.size());

    for (auto i = 0ul; i < tags.size(); ++i) {
        EXPECT_EQ(post.tags[i], tags[i]) << "at index " << i;
    }
}
