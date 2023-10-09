#include "database.test.hpp"

TEST_F(DatabaseTagTest, CreateTag) {
    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();
        const auto tag = (co_await db->read_tag(id)).value();

        EXPECT_EQ(id, tag.id);
        EXPECT_EQ(tag_name, tag.name);
        EXPECT_TRUE(tag.aliases.empty());
        EXPECT_TRUE(tag.description.empty());
        EXPECT_FALSE(tag.avatar);
        EXPECT_FALSE(tag.banner);
    }());
}
