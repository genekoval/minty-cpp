#include "database.test.hpp"

TEST_F(DatabaseObjectTest, ReadTotalObjectsNone) {
    run([&]() -> ext::task<> {
        const auto total = co_await db->read_total_objects();
        EXPECT_EQ(0, total);
    }());
}

TEST_F(DatabasePostObjectTest, ReadTotalObjectsMultiple) {
    run([&]() -> ext::task<> {
        const auto total = co_await db->read_total_objects();
        // Add one to account for 'new_object' being added.
        EXPECT_EQ(objects.size() + 1, total);
    }());
}
