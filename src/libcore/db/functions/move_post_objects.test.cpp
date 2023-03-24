#include "database.test.hpp"

TEST_F(DatabasePostObjectTest, MovePostObjectsOneToBeginning) {
    run([&]() -> ext::task<> {
        co_await db->move_post_objects(
            post_id,
            {objects.back()},
            objects.front()
        );

        const auto result = co_await with_sequence();

        EXPECT_EQ(3, result.size());

        EXPECT_EQ(objects.back(), result.at(0).id);
        EXPECT_EQ(objects.front(), result.at(1).id);
        EXPECT_EQ(objects[1], result.at(2).id);

        EXPECT_EQ(1, result.at(0).sequence);
        EXPECT_EQ(2, result.at(1).sequence);
        EXPECT_EQ(3, result.at(2).sequence);
    }());
}

TEST_F(DatabasePostObjectTest, MovePostObjectsOneToEnd) {
    run([&]() -> ext::task<> {
        co_await db->move_post_objects(post_id, {objects.front()}, {});

        const auto result = co_await with_sequence();

        EXPECT_EQ(3, result.size());

        EXPECT_EQ(objects.front(), result.at(2).id);
        EXPECT_EQ(objects[1], result.at(0).id);
        EXPECT_EQ(objects.back(), result.at(1).id);

        EXPECT_EQ(1, result.at(0).sequence);
        EXPECT_EQ(2, result.at(1).sequence);
        EXPECT_EQ(3, result.at(2).sequence);
    }());
}

TEST_F(DatabasePostObjectTest, MovePostObjectsMultiple) {
    run([&]() -> ext::task<> {
        co_await db->move_post_objects(
            post_id,
            {objects[1], objects[2]},
            objects.front()
        );

        const auto result = co_await with_sequence();

        EXPECT_EQ(3, result.size());

        EXPECT_EQ(objects[1], result.at(0).id);
        EXPECT_EQ(objects[2], result.at(1).id);
        EXPECT_EQ(objects[0], result.at(2).id);

        EXPECT_EQ(1, result.at(0).sequence);
        EXPECT_EQ(2, result.at(1).sequence);
        EXPECT_EQ(3, result.at(2).sequence);
    }());
}

TEST_F(DatabasePostObjectTest, MovePostObjectsDateModified) {
    run([&]() -> ext::task<> {
        const auto date_modified = co_await db->move_post_objects(
            post_id,
            {objects.back()},
            objects.front()
        );

        const auto post = co_await db->read_post(post_id);

        EXPECT_EQ(date_modified, post.date_modified);
        EXPECT_NE(post.date_created, post.date_modified);
    }());
}
