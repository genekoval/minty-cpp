#include "search.test.h"

TEST_F(SearchPostTest, DeletePost) {
    netcore::run([&]() -> ext::task<> {
        const auto& post = (co_await add_post()).get();

        const auto exists = [&]() -> ext::task<bool> {
            co_return co_await client.doc_exists(index, post.id).send();
        };

        EXPECT_TRUE(co_await exists());

        co_await search.delete_post(post.id);

        EXPECT_FALSE(co_await exists());
    }());
}
