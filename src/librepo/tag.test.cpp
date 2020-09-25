#include "db.test.h"

class DatabaseTagTest : public DatabaseTest {
protected:
    auto tables() -> std::vector<std::string> override {
        return {"tag"};
    }
};

TEST_F(DatabaseTagTest, CreateTag) {
    constexpr auto name = "mytag";
    constexpr auto color = "0f0f0f";

    const auto tag = database.create_tag(name, color);

    ASSERT_EQ(name, tag.name);
    ASSERT_EQ(color, tag.color);
}
