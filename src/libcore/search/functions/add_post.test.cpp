#include "search.test.h"

TEST_F(SearchPostTest, AddPost) {
    const auto& post = add_post();
    auto res = get_post(post.id);

    const auto title = std::string_view(res["title"]);
    const auto description = std::string_view(res["description"]);

    const auto created = get_time(res, "created");
    const auto modified = get_time(res, "modified");

    auto tags = std::vector<std::string_view>();
    for (auto tag : res["tags"].get_array()) {
        tags.emplace_back(tag);
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
