#include "search.test.hpp"

TEST_F(SearchTagTest, DeleteTag) {
    netcore::run([&]() -> ext::task<> {
        const auto& tag = (co_await add_tag()).get();

        const auto exists = [&]() -> ext::task<bool> {
            co_return co_await client.doc_exists(index, tag.id).send();
        };

        EXPECT_TRUE(co_await exists());

        co_await search.delete_tag(tag.id);

        EXPECT_FALSE(co_await exists());
    }());
}
