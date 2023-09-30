#include "database.test.hpp"

TEST_F(DatabaseTagTest, DeleteTagAlias) {
    constexpr auto alias = "Alias";

    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();

        co_await db->create_tag_alias(id, alias);
        auto tag = (co_await db->read_tag(id)).value();

        EXPECT_EQ(1, tag.aliases.size());

        co_await db->delete_tag_alias(id, alias);
        tag = (co_await db->read_tag(id)).value();

        EXPECT_TRUE(tag.aliases.empty());
    }());
}
