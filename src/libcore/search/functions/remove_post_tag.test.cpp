#include "search.test.h"

TEST_F(SearchPostTest, RemovePostTag) {
    auto opt = std::optional<std::reference_wrapper<const post>>();
    auto tags = std::vector<UUID::uuid>();

    netcore::run([&]() -> ext::task<> {
        opt = co_await add_post();
        const auto& post = opt->get();

        co_await search.remove_post_tag(post.id, post.tags.front());

        const auto result = co_await get_post(post.id);
        tags = result["tags"].get<std::vector<UUID::uuid>>();
    }());

    const auto& post = opt->get();

    ASSERT_EQ(post.tags.size() - 1, tags.size());

    for (auto i = 0ul; i < tags.size(); ++i) {
        EXPECT_EQ(post.tags.at(i + 1), tags.at(i)) << " at index " << i;
    }
}
