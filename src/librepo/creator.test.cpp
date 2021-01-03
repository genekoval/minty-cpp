#include "db.test.h"

#include <minty/repo/db/model.h>

class DatabaseCreatorTest : public DatabaseTest {
protected:
    static constexpr auto creator_name = "minty";

    auto create_creator() -> std::string {
        return database.create_creator(creator_name);
    }

    auto tables() -> std::vector<std::string> override {
        return {
            "creator",
            "site"
        };
    }
};

TEST_F(DatabaseCreatorTest, Create) {
    const auto id = create_creator();
    const auto creator = database.read_creator(id);

    ASSERT_EQ(id, creator.id);
    ASSERT_EQ(creator_name, creator.name);
    ASSERT_TRUE(creator.aliases.empty());
    ASSERT_FALSE(creator.bio.has_value());
    ASSERT_FALSE(creator.avatar.has_value());
    ASSERT_FALSE(creator.banner.has_value());
    ASSERT_TRUE(creator.sources.empty());
}

TEST_F(DatabaseCreatorTest, CreateAliases) {
    const auto aliases = std::vector<std::string>{
        "banana",
        "citrus",
        "apple"
    };

    const auto id = create_creator();

    database.create_creator_aliases(id, aliases);

    const auto creator = database.read_creator(id);

    ASSERT_EQ(creator_name, creator.name);
    ASSERT_EQ(aliases.size(), creator.aliases.size());

    ASSERT_EQ("apple", creator.aliases[0]);
    ASSERT_EQ("banana", creator.aliases[1]);
    ASSERT_EQ("citrus", creator.aliases[2]);
}

TEST_F(DatabaseCreatorTest, CreateSource) {
    constexpr auto url = "https://example.com/cats";
    const auto id = create_creator();
    const auto site = database.create_site(
        "Example",
        "https://example.com",
        "937900e4-54a0-40fb-8ac8-315e5d3b2ae1"
    );

    database.create_creator_source(id, site.id, url);

    const auto creator = database.read_creator(id);

    ASSERT_EQ(1, creator.sources.size());
    ASSERT_EQ(url, creator.sources[0].url);
    ASSERT_EQ(site.id, creator.sources[0].website.id);
}
