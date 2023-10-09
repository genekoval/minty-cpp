#include "search.test.hpp"

TEST_F(SearchPostTest, UpdatePostTitle) {
    constexpr auto title = "A New Title";
    const auto modified = minty::test::now();

    netcore::run([&]() -> ext::task<> {
        const auto& post = (co_await add_post()).get();

        co_await search.update_post_title(post.id, title, modified);

        const auto res = co_await get_post(post.id);

        EXPECT_EQ(title, res["title"].get<std::string>());
        EXPECT_EQ(modified, res["modified"].get<minty::time_point>());
    }());
}
