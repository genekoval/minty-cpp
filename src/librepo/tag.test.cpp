#include "db.test.h"

#include <minty/repo/db/model.h>

class DatabaseTagTest : public DatabaseTest {
protected:
    static constexpr auto tag_name = "minty";

    auto create_tag() -> std::string {
        return database.create_tag(tag_name);
    }

    auto tables() -> std::vector<std::string> override {
        return {
            "tag",
            "site"
        };
    }
};

TEST_F(DatabaseTagTest, Create) {
    const auto id = create_tag();
    const auto tag = database.read_tag(id);

    ASSERT_EQ(id, tag.id);
    ASSERT_EQ(tag_name, tag.name);
    ASSERT_TRUE(tag.aliases.empty());
    ASSERT_FALSE(tag.description.has_value());
    ASSERT_FALSE(tag.avatar.has_value());
    ASSERT_FALSE(tag.banner.has_value());
    ASSERT_TRUE(tag.sources.empty());
}

TEST_F(DatabaseTagTest, CreateAliases) {
    const auto aliases = std::vector<std::string>{
        "banana",
        "citrus",
        "apple"
    };

    const auto id = create_tag();

    database.create_tag_aliases(id, aliases);

    const auto tag = database.read_tag(id);

    ASSERT_EQ(tag_name, tag.name);
    ASSERT_EQ(aliases.size(), tag.aliases.size());

    ASSERT_EQ("apple", tag.aliases[0]);
    ASSERT_EQ("banana", tag.aliases[1]);
    ASSERT_EQ("citrus", tag.aliases[2]);
}

TEST_F(DatabaseTagTest, CreateSource) {
    constexpr auto url = "https://example.com/cats";
    const auto id = create_tag();
    const auto site = database.create_site(
        "Example",
        "https://example.com",
        "937900e4-54a0-40fb-8ac8-315e5d3b2ae1"
    );

    database.create_tag_source(id, site.id, url);

    const auto tag = database.read_tag(id);

    ASSERT_EQ(1, tag.sources.size());
    ASSERT_EQ(url, tag.sources[0].url);
    ASSERT_EQ(site.id, tag.sources[0].website.id);
}

TEST_F(DatabaseTagTest, DeleteTag) {
    const auto tag1 = create_tag();
    const auto tag2 = create_tag();

    ASSERT_EQ(2, count("tag"));

    database.delete_tag(tag1);

    ASSERT_EQ(1, count("tag"));

    const auto remaining = database.read_tag(tag2);
    ASSERT_EQ(tag2, remaining.id);
}
