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

TEST_F(DatabaseTagTest, CreateTag) {
    const auto id = create_tag();
    const auto tag = database.read_tag(id);

    ASSERT_EQ(id, tag.id);
    ASSERT_EQ(tag_name, tag.name);
    ASSERT_TRUE(tag.aliases.empty());
    ASSERT_FALSE(tag.description.has_value());
    ASSERT_FALSE(tag.avatar.has_value());
    ASSERT_FALSE(tag.banner.has_value());
}

TEST_F(DatabaseTagTest, CreateAliases) {
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

TEST_F(DatabaseTagTest, CreateSource) {
    constexpr auto resource = "/cats?v=100";

    const auto id = create_tag();
    const auto site = database.create_site(
        "https",
        "example.com",
        "937900e4-54a0-40fb-8ac8-315e5d3b2ae1"
    );

    const auto source = database.create_source(site.id, resource);
    database.create_tag_source(id, source.id);

    const auto sources = database.read_tag_sources(id);

    ASSERT_EQ(1, sources.size());

    const auto& src = sources.front();

    ASSERT_EQ(resource, src.resource);
    ASSERT_EQ(site.id, src.website.id);
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

TEST_F(DatabaseTagTest, DeleteAlias) {
    constexpr auto alias = "Alias";

    const auto id = create_tag();

    database.create_tag_alias(id, alias);
    auto tag = database.read_tag(id);

    ASSERT_EQ(1, tag.aliases.size());

    database.delete_tag_alias(id, alias);
    tag = database.read_tag(id);

    ASSERT_TRUE(tag.aliases.empty());
}

TEST_F(DatabaseTagTest, UpdateDescription) {
    constexpr auto description = "This is a test description.";

    const auto id = create_tag();

    auto tag = database.read_tag(id);
    ASSERT_FALSE(tag.description.has_value());

    const auto desc = database.update_tag_description(id, description);
    ASSERT_TRUE(desc.has_value());
    ASSERT_EQ(description, desc.value());

    tag = database.read_tag(id);
    ASSERT_TRUE(tag.description.has_value());
    ASSERT_EQ(description, tag.description.value());
}

TEST_F(DatabaseTagTest, UpdateDescriptionEmpty) {
    constexpr auto description = "";

    const auto id = create_tag();

    const auto desc = database.update_tag_description(id, description);
    ASSERT_FALSE(desc.has_value());

    const auto tag = database.read_tag(id);
    ASSERT_FALSE(tag.description.has_value());
}

TEST_F(DatabaseTagTest, UpdateName) {
    constexpr auto new_name = "New Name";

    const auto id = create_tag();
    const auto [names, old_name] = database.update_tag_name(id, new_name);

    ASSERT_TRUE(old_name.has_value());
    ASSERT_EQ(tag_name, *old_name);
    ASSERT_EQ(new_name, names.name);
    ASSERT_TRUE(names.aliases.empty());
}

TEST_F(DatabaseTagTest, UpdateNameReplace) {
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

TEST_F(DatabaseTagTest, UpdateNameSwap) {
    constexpr auto alias = "Other Name";

    const auto id = create_tag();
    database.create_tag_alias(id, alias);
    const auto [names, old_name] = database.update_tag_name(id, alias);

    ASSERT_FALSE(old_name.has_value());
    ASSERT_EQ(alias, names.name);
    ASSERT_EQ(tag_name, names.aliases.front());
}
