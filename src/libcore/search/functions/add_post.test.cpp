#include "search.test.hpp"

TEST_F(SearchPostTest, AddPost) {
    netcore::run([&]() -> ext::task<> {
        const auto& post = (co_await add_post()).get();
        auto res = co_await get_post(post.id);

        const auto title = res.at("title").get<std::string>();
        const auto description = res.at("description").get<std::string>();

        const auto visibility = res.at("visibility").get<minty::visibility>();

        const auto created = res.at("created").get<minty::time_point>();
        const auto modified = res.at("modified").get<minty::time_point>();

        auto tags = std::vector<UUID::uuid>();
        for (const auto& tag : res.at("tags")) {
            tags.push_back(tag.get<UUID::uuid>());
        }

        EXPECT_EQ(post.title, title);
        EXPECT_EQ(post.description, description);

        EXPECT_EQ(post.visibility, visibility);

        EXPECT_EQ(post.created, created);
        EXPECT_EQ(post.modified, modified);

        EXPECT_EQ(post.tags.size(), tags.size());

        for (auto i = 0ul; i < tags.size(); ++i) {
            EXPECT_EQ(post.tags.at(i), tags.at(i)) << "at index " << i;
        }
    }());
}
