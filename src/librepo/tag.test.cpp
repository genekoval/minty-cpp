#include "db.test.h"

#include <minty/error.h>

class DatabaseTagTest : public DatabaseTest {
protected:
    auto tables() -> std::vector<std::string> override {
        return {"tag"};
    }
};

TEST_F(DatabaseTagTest, Create) {
    constexpr auto name = "tag";
    constexpr auto color = "0f0f0f";

    const auto tag = database.create_tag(name, color);

    ASSERT_EQ(name, tag.name);
    ASSERT_EQ(color, tag.color);
}

TEST_F(DatabaseTagTest, CreateDuplicateName) {
    const auto tag = database.create_tag("tag", "0f0f0f");

    try {
        database.create_tag(tag.name, "2b2b2b");
        FAIL() << "Created a tag with an existing name.";
    }
    catch (const minty::unique_entity_violation& ex) {
        ASSERT_EQ(tag.name, ex.value);
    }
}

TEST_F(DatabaseTagTest, CreateDuplicateColor) {
    const auto tag1 = database.create_tag("tag1", "0f0f0f");
    const auto tag2 = database.create_tag("tag2", tag1.color);

    ASSERT_EQ(tag1.color, tag2.color);
}
