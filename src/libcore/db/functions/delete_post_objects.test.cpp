#include "database.test.hpp"

TEST_F(DatabasePostObjectTest, DeletePostObjects) {
    run([&]() -> ext::task<> {
        co_await db->delete_post_objects(post_id, {objects[1], objects[2]});

        const auto result = co_await db->read_post_objects(post_id);

        EXPECT_EQ(1, result.size());
        EXPECT_EQ(objects.front(), result.at(0).id);
    }());
}

TEST_F(DatabasePostObjectTest, DeletePostObjectsDateModified) {
    run([&]() -> ext::task<> {
        const auto date_modified = co_await db->delete_post_objects(
            post_id,
            {objects.front()}
        );

        const auto post = co_await db->read_post(post_id);

        EXPECT_EQ(date_modified, post.date_modified);
        EXPECT_NE(post.date_created, post.date_modified);
    }());
}

TEST_F(DatabasePostObjectTest, DeletePostObjectsSequence) {
    run([&]() -> ext::task<> {
        co_await db->delete_post_objects(post_id, {objects.front()});

        const auto result = co_await with_sequence();

        EXPECT_EQ(objects[1], result[0].id);
        EXPECT_EQ(1, result[0].sequence);

        EXPECT_EQ(objects[2], result[1].id);
        EXPECT_EQ(2, result[1].sequence);
    }());
}
