#include "search.test.hpp"

TEST_F(SearchPostTest, UpdatePostDescription) {
    constexpr auto description = "A new description.";
    const auto modified = minty::test::now();

    netcore::run([&]() -> ext::task<> {
        const auto& post = (co_await add_post()).get();

        const auto update = minty::core::db::post_update {
            .id = post.id,
            .new_data = description,
            .date_modified = modified
        };

        co_await search.update_post_description(update);

        const auto res = co_await get_post(post.id);

        EXPECT_EQ(description, res["description"].get<std::string>());
        EXPECT_EQ(modified, res["modified"].get<minty::time_point>());
    }());
}
