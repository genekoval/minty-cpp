#include "database.test.hpp"

TEST_F(DatabaseTagTest, UpdateTagDescription) {
    constexpr auto description = "This is a test description.";

    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();
        auto tag = (co_await db->read_tag(id)).value();

        EXPECT_TRUE(co_await db->update_tag_description(id, description));

        tag = (co_await db->read_tag(id)).value();
        EXPECT_EQ(description, tag.description);
    }());
}

TEST_F(DatabaseTagTest, UpdateTagDescriptionNotFound) {
    run([&]() -> ext::task<> {
        const auto id = UUID::generate();
        EXPECT_FALSE(co_await db->update_tag_description(id, ""));
    }());
}
