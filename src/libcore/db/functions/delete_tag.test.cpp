#include "database.test.hpp"

TEST_F(DatabaseTagTest, DeleteTag) {
    run([&]() -> ext::task<> {
        const auto tag1 = co_await create_tag();
        const auto tag2 = co_await create_tag();

        EXPECT_EQ(2, co_await count("tag"));

        co_await db->delete_tag(tag1);

        EXPECT_EQ(1, co_await count("tag"));

        const auto remaining = co_await db->read_tag(tag2);
        EXPECT_EQ(tag2, remaining.id);
    }());
}
