#include "database.test.hpp"

TEST_F(DatabaseTagTest, UpdateTagName) {
    constexpr auto new_name = "New Name";

    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();
        const auto [names, old_name] =
            co_await db->update_tag_name(id, new_name);

        EXPECT_TRUE(old_name.has_value());
        EXPECT_EQ(tag_name, old_name.value());
        EXPECT_EQ(new_name, names.name);
        EXPECT_TRUE(names.aliases.empty());
    }());
}

TEST_F(DatabaseTagTest, UpdateTagNameReplace) {
    constexpr auto new_name = "New Name";
    constexpr auto alias = "Hello World";

    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();
        co_await db->create_tag_alias(id, alias);
        const auto [names, old_name] =
            co_await db->update_tag_name(id, new_name);

        EXPECT_TRUE(old_name.has_value());
        EXPECT_EQ(tag_name, *old_name);
        EXPECT_EQ(new_name, names.name);
        EXPECT_EQ(alias, names.aliases.front());
    }());
}

TEST_F(DatabaseTagTest, UpdateTagNameSwap) {
    constexpr auto alias = "Other Name";

    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();
        co_await db->create_tag_alias(id, alias);
        const auto [names, old_name] = co_await db->update_tag_name(id, alias);

        EXPECT_FALSE(old_name.has_value());
        EXPECT_EQ(alias, names.name);
        EXPECT_EQ(tag_name, names.aliases.front());
    }());
}
