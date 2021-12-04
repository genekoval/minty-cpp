#include "database.test.h"

TEST_F(DatabaseTagTest, DeleteTag) {
    const auto tag1 = create_tag();
    const auto tag2 = create_tag();

    ASSERT_EQ(2, count("tag"));

    database.delete_tag(tag1);

    ASSERT_EQ(1, count("tag"));

    const auto remaining = database.read_tag(tag2);
    ASSERT_EQ(tag2, remaining.id);
}
