#include "database.test.hpp"

TEST_F(DatabaseTagTest, UpdateTagDescription) {
    constexpr auto description = "This is a test description.";

    const auto id = create_tag();

    auto tag = database.read_tag(id);

    const auto desc = database.update_tag_description(id, description);
    ASSERT_TRUE(desc.has_value());
    ASSERT_EQ(description, desc.value());

    tag = database.read_tag(id);
    ASSERT_TRUE(tag.description.has_value());
    ASSERT_EQ(description, tag.description.value());
}

TEST_F(DatabaseTagTest, UpdateTagDescriptionEmpty) {
    constexpr auto description = "";

    const auto id = create_tag();

    const auto desc = database.update_tag_description(id, description);
    ASSERT_FALSE(desc.has_value());

    const auto tag = database.read_tag(id);
    ASSERT_FALSE(tag.description.has_value());
}
