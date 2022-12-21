#include "database.test.hpp"

TEST_F(DatabaseTagTest, CreateTag) {
    const auto id = create_tag();
    const auto tag = database.read_tag(id);

    ASSERT_EQ(id, tag.id);
    ASSERT_EQ(tag_name, tag.name);
    ASSERT_TRUE(tag.aliases.empty());
    ASSERT_FALSE(tag.description);
    ASSERT_FALSE(tag.avatar);
    ASSERT_FALSE(tag.banner);
}
