#include "search.test.h"

TEST_F(SearchPostTest, UpdatePostDateModified) {
    const auto& post = add_post();
    const auto modified = minty::test::now();

    search.update_post_date_modified(post.id, modified);

    auto res = get_post(post.id);
    const auto result = get_time(res, "modified");

    ASSERT_EQ(modified, result);
}
