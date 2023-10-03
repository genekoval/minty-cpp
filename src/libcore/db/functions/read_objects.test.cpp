#include "database.test.hpp"

TEST_F(DatabasePostObjectTest, ReadObjects) {
    run([&]() -> ext::task<> {
        auto found = std::vector<UUID::uuid>();

        auto portal = co_await db->read_objects(2);

        while (portal) {
            const auto objects = co_await portal.next();

            for (const auto& object : objects) { found.push_back(object.id); }
        }

        EXPECT_EQ(4, found.size());

        EXPECT_EQ(objects[0], found.at(0));
        EXPECT_EQ(objects[1], found.at(1));
        EXPECT_EQ(new_object, found.at(2));
        EXPECT_EQ(objects[2], found.at(3));
    }());
}
