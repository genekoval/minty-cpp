#include "search.test.h"

TEST_F(SearchPostTest, AddPost) {
    netcore::run([&]() -> ext::task<> {
        const auto& post = (co_await add_post()).get();
        auto res = co_await get_post(post.id);

        const auto title = res["title"].get<std::string>();
        const auto description = res["description"].get<std::string>();

        const auto created = res["created"].get<minty::test::time_point>();
        const auto modified = res["modified"].get<minty::test::time_point>();

        auto tags = std::vector<UUID::uuid>();
        for (const auto& tag : res["tags"]) {
            tags.push_back(tag.get<UUID::uuid>());
        }

        EXPECT_EQ(post.title, title);
        EXPECT_EQ(post.description, description);

        EXPECT_EQ(post.created, created);
        EXPECT_EQ(post.modified, modified);

        EXPECT_EQ(post.tags.size(), tags.size());

        for (auto i = 0ul; i < tags.size(); ++i) {
            EXPECT_EQ(post.tags[i], tags[i]) << "at index " << i;
        }
    }());
}
