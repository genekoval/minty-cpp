#include "search.test.hpp"

TEST_F(SearchPostTest, AddPostTag) {
    auto post_opt = std::optional<std::reference_wrapper<const post>>();
    auto tags = std::vector<UUID::uuid>();

    netcore::run([&]() -> ext::task<> {
        post_opt = co_await add_post();
        const auto& post = post_opt->get();

        co_await search.add_post_tag(post.id, tag);

        const auto res = co_await get_post(post.id);
        tags = res["tags"].get<std::vector<UUID::uuid>>();
    }());

    const auto& post = post_opt->get();

    ASSERT_EQ(post.tags.size() + 1, tags.size());

    for (auto i = 0ul; i < post.tags.size(); ++i) {
        EXPECT_EQ(post.tags.at(i), tags.at(i)) << "at index " << i;
    }

    EXPECT_EQ(tag, tags.back());
}
