#include "search.test.h"

TEST_F(SearchPostTest, UpdatePostDateModified) {
    const auto& post = add_post();
    const auto modified = minty::test::now();

    search.update_post_date_modified(post.id, modified);

    const auto result =
        get_post(post.id)["modified"].get<minty::test::time_point>();

    ASSERT_EQ(modified, result);
}
