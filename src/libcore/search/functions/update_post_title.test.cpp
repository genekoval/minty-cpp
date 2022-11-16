#include "search.test.h"

TEST_F(SearchPostTest, UpdatePostTitle) {
    constexpr auto title = "A New Title";
    const auto modified = minty::test::now();

    const auto& post = add_post();

    const auto update = minty::core::post_update {
        .id = post.id,
        .new_data = title,
        .date_modified = modified
    };

    search.update_post_title(update);

    auto res = get_post(post.id);

    ASSERT_EQ(title, res["title"].get<std::string>());
    ASSERT_EQ(modified, res["modified"].get<minty::test::time_point>());
}
