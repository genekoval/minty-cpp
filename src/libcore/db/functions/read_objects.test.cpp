#include "database.test.hpp"

TEST_F(DatabasePostObjectTest, ReadObjects) {
    auto found = std::vector<UUID::uuid>();

    auto objects = database.read_objects(2);

    while (objects) {
        for (auto&& object : objects()) {
            found.push_back(object.id);
        }
    }

    ASSERT_EQ(4, found.size());

    ASSERT_EQ(this->objects[0], found[0]);
    ASSERT_EQ(this->objects[1], found[1]);
    ASSERT_EQ(new_object, found[2]);
    ASSERT_EQ(this->objects[2], found[3]);
}
