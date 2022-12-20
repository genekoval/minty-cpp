#include "search.test.h"

TEST_F(SearchPostTest, UpdatePostDateModified) {
    netcore::run([&]() -> ext::task<> {
        const auto& post = (co_await add_post()).get();
        const auto modified = minty::test::now();

        co_await search.update_post_date_modified(post.id, modified);

        const auto res = co_await get_post(post.id);
        const auto result = res["modified"].get<minty::time_point>();

        EXPECT_EQ(modified, result);
    }());
}
