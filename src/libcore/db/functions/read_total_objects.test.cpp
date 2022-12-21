#include "database.test.hpp"

TEST_F(DatabaseObjectTest, ReadTotalObjectsNone) {
    auto total = database.read_total_objects();
    ASSERT_EQ(0, total);
}

TEST_F(DatabasePostObjectTest, ReadTotalObjectsMultiple) {
    auto total = database.read_total_objects();
    // Add one to account for 'new_object' being added.
    ASSERT_EQ(objects.size() + 1, total);
}
