#include "database.test.hpp"

TEST_F(DatabaseTagTest, UpdateTagName) {
    constexpr auto new_name = "New Name";

    const auto id = create_tag();
    const auto [names, old_name] = database.update_tag_name(id, new_name);

    ASSERT_TRUE(old_name.has_value());
    ASSERT_EQ(tag_name, old_name.value());
    ASSERT_EQ(new_name, names.name);
    ASSERT_TRUE(names.aliases.empty());
}

TEST_F(DatabaseTagTest, UpdateTagNameReplace) {
    constexpr auto new_name = "New Name";
    constexpr auto alias = "Hello World";

    const auto id = create_tag();
    database.create_tag_alias(id, alias);
    const auto [names, old_name] = database.update_tag_name(id, new_name);

    ASSERT_TRUE(old_name.has_value());
    ASSERT_EQ(tag_name, *old_name);
    ASSERT_EQ(new_name, names.name);
    ASSERT_EQ(alias, names.aliases.front());
}

TEST_F(DatabaseTagTest, UpdateTagNameSwap) {
    constexpr auto alias = "Other Name";

    const auto id = create_tag();
    database.create_tag_alias(id, alias);
    const auto [names, old_name] = database.update_tag_name(id, alias);

    ASSERT_FALSE(old_name.has_value());
    ASSERT_EQ(alias, names.name);
    ASSERT_EQ(tag_name, names.aliases.front());
}
