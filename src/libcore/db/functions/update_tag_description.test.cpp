#include "database.test.hpp"

TEST_F(DatabaseTagTest, UpdateTagDescription) {
    constexpr auto description = "This is a test description.";

    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();
        auto tag = co_await db->read_tag(id);

        const auto desc = co_await db->update_tag_description(id, description);
        EXPECT_TRUE(desc.has_value());
        EXPECT_EQ(description, desc.value());

        tag = co_await db->read_tag(id);
        EXPECT_TRUE(tag.description.has_value());
        EXPECT_EQ(description, tag.description.value());
    }());
}

TEST_F(DatabaseTagTest, UpdateTagDescriptionEmpty) {
    constexpr auto description = "";

    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();

        const auto desc = co_await db->update_tag_description(id, description);
        EXPECT_FALSE(desc.has_value());

        const auto tag = co_await db->read_tag(id);
        EXPECT_FALSE(tag.description.has_value());
    }());
}
