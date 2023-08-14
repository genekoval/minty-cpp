#include "database.test.hpp"

TEST_F(DatabasePostObjectTest, DeletePostObjects) {
    run([&]() -> ext::task<> {
        co_await db->delete_post_objects(post_id, {objects[1], objects[2]});

        const auto post = co_await db->read_post(post_id);
        const auto& objects = post.objects;

        EXPECT_EQ(1, objects.size());
        EXPECT_EQ(this->objects.front(), objects.at(0).id);
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

        const auto post = co_await db->read_post(post_id);
        const auto& result = post.objects;

        EXPECT_EQ(objects.at(1), result.at(0).id);
        EXPECT_EQ(objects.at(2), result.at(1).id);
    }());
}
