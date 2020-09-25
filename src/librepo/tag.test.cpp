#include "db.test.h"

class DatabaseTagTest : public DatabaseTest {
protected:
    DatabaseTagTest() { truncate("tag"); }

    ~DatabaseTagTest() { truncate("tag"); }
};

TEST_F(DatabaseTagTest, CreateTag) {
    constexpr auto name = "mytag";
    constexpr auto color = "0f0f0f";

    const auto tag = db.create_tag(name, color);

    ASSERT_EQ(name, tag.name);
    ASSERT_EQ(color, tag.color);
}
