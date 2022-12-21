#include "database.test.hpp"

TEST_F(DatabaseObjectTest, CreateObject) {
    database.create_object(object_id, {}, {});
    ASSERT_EQ(1, count("object"));

    database.create_object(object_id, {}, {});
    ASSERT_EQ(1, count("object"));

    const auto object = database.read_object(object_id);

    ASSERT_EQ(object_id, object.id);
    ASSERT_FALSE(object.preview_id.has_value());
    ASSERT_FALSE(object.src.has_value());
}
