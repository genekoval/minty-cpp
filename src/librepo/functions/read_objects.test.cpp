#include "database.test.h"

TEST_F(DatabasePostObjectTest, ReadObjects) {
    auto found = std::vector<UUID::uuid>();

    database.read_objects(2, [&found](auto objects) {
        for (const auto& object : objects) found.push_back(object.id);
    });

    ASSERT_EQ(4, found.size());

    ASSERT_EQ(objects[0], found[0]);
    ASSERT_EQ(objects[1], found[1]);
    ASSERT_EQ(new_object, found[2]);
    ASSERT_EQ(objects[2], found[3]);
}
