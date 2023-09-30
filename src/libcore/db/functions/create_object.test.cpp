#include "database.test.hpp"

TEST_F(DatabaseObjectTest, CreateObject) {
    run([&]() -> ext::task<> {
        co_await db->create_object(object_id, {}, {});
        EXPECT_EQ(1, co_await count("object"));

        co_await db->create_object(object_id, {}, {});
        EXPECT_EQ(1, co_await count("object"));

        const auto object = (co_await db->read_object(object_id)).value();

        EXPECT_EQ(object_id, object.id);
        EXPECT_FALSE(object.preview_id.has_value());
        EXPECT_FALSE(object.src.has_value());
    }());
}
