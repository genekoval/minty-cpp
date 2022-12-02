#include "search.test.h"

TEST_F(SearchPostTest, AddPosts) {
    netcore::run([&]() -> ext::task<> {
        const auto errors = co_await search.add_posts(posts);
        EXPECT_TRUE(errors.empty());

        co_await index.refresh();

        const auto post_count = static_cast<unsigned int>(posts.size());

        const auto result = co_await search.find_posts({.size = post_count});
        const auto& hits = result.hits;

        EXPECT_EQ(post_count, result.total);
        EXPECT_EQ(result.total, hits.size());

        for (const auto& post : posts) {
            const auto& id = post.id;
            const auto end = hits.end();
            const auto contains = std::find(hits.begin(), end, id) != end;

            EXPECT_TRUE(contains) << "result does not contain post [" << id << "]";
        }
    }());
}

TEST_F(SearchPostTest, AddPostsErrors) {
    netcore::run([&]() -> ext::task<> {
        auto errors = co_await search.add_posts(posts);
        EXPECT_TRUE(errors.empty());

        co_await index.refresh();

        constexpr auto duplicates = 3;

        errors = co_await search.add_posts(std::span<const post>(
            posts.begin(),
            posts.begin() + duplicates
        ));

        EXPECT_EQ(duplicates, errors.size());
    }());
}
