#include "search.test.hpp"

TEST_F(SearchPostTest, UpdatePostTitle) {
    constexpr auto title = "A New Title";
    const auto modified = minty::test::now();

    netcore::run([&]() -> ext::task<> {
        const auto& post = (co_await add_post()).get();

        const auto update = minty::repo::db::post_update {
            .id = post.id,
            .new_data = title,
            .date_modified = modified
        };

        co_await search.update_post_title(update);

        const auto res = co_await get_post(post.id);

        EXPECT_EQ(title, res["title"].get<std::string>());
        EXPECT_EQ(modified, res["modified"].get<minty::time_point>());
    }());
}
