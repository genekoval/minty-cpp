#include "database.test.hpp"

TEST_F(DatabaseTagTest, CreateTagAliases) {
    const auto aliases = std::vector<std::string>{
        "banana",
        "citrus",
        "apple"
    };

    const auto id = create_tag();

    for (const auto& alias : aliases) {
        database.create_tag_alias(id, alias);
    }

    const auto tag = database.read_tag(id);

    ASSERT_EQ(tag_name, tag.name);
    ASSERT_EQ(aliases.size(), tag.aliases.size());

    ASSERT_EQ("apple", tag.aliases[0]);
    ASSERT_EQ("banana", tag.aliases[1]);
    ASSERT_EQ("citrus", tag.aliases[2]);
}
