#include "search.test.hpp"

TEST_F(SearchPostTest, UpdatePostDescription) {
    constexpr auto description = "A new description.";
    const auto modified = minty::test::now();

    netcore::run([&]() -> ext::task<> {
        const auto& post = (co_await add_post()).get();

        co_await search.update_post_description(post.id, description, modified);

        const auto res = co_await get_post(post.id);

        EXPECT_EQ(description, res["description"].get<std::string>());
        EXPECT_EQ(modified, res["modified"].get<minty::time_point>());
    }());
}
