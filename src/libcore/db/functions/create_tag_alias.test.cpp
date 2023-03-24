#include "database.test.hpp"

TEST_F(DatabaseTagTest, CreateTagAliases) {
    constexpr auto aliases = std::array<std::string_view, 3> {
        "banana",
        "citrus",
        "apple"
    };

    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();

        for (const auto& alias : aliases) {
            co_await db->create_tag_alias(id, alias);
        }

        const auto tag = co_await db->read_tag(id);

        EXPECT_EQ(tag_name, tag.name);
        EXPECT_EQ(aliases.size(), tag.aliases.size());

        EXPECT_EQ("apple", tag.aliases.at(0));
        EXPECT_EQ("banana", tag.aliases.at(1));
        EXPECT_EQ("citrus", tag.aliases.at(2));
    }());
}
