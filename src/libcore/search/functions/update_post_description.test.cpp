#include "search.test.h"

TEST_F(SearchPostTest, UpdatePostDescription) {
    constexpr auto description = "A new description.";
    const auto modified = minty::test::now();

    const auto& post = add_post();

    const auto update = minty::core::post_update {
        .id = post.id,
        .new_data = description,
        .date_modified = modified
    };

    search.update_post_description(update);

    auto res = get_post(post.id);

    ASSERT_EQ(description, std::string_view(res["description"]));
    ASSERT_EQ(modified, get_time(res, "modified"));
}
