#include "database.test.h"

TEST_F(DatabasePostObjectTest, AppendPostObject) {
    const auto results = insert_object(objects.size());

    ASSERT_EQ(4, results.size());

    for (auto i = 1u; i <= objects.size(); ++i) {
        const auto object = objects[i - 1];
        const auto result = results[i - 1];

        ASSERT_EQ(object, result.id);
        ASSERT_EQ(i, result.sequence);
    }

    ASSERT_EQ(new_object, results.back().id);
    ASSERT_EQ(4, results.back().sequence);
}

TEST_F(DatabasePostObjectTest, InsertPostObject) {
    const auto results = insert_object(1);

    ASSERT_EQ(4, results.size());

    ASSERT_EQ(objects[0], results[0].id);
    ASSERT_EQ(new_object, results[1].id);
    ASSERT_EQ(objects[1], results[2].id);
    ASSERT_EQ(objects[2], results[3].id);

    for (auto i = 1u; i < results.size(); ++i) {
        ASSERT_EQ(i, results[i - 1].sequence);
    }
}

TEST_F(DatabasePostObjectTest, InsertMultiplePostObjects) {
    const auto additional = "c3563941-473d-4ed1-9ea8-ef9569642443";
    database.create_object(additional, {}, {});

    const auto results = insert_objects({new_object, additional}, 1);

    ASSERT_EQ(5, results.size());

    ASSERT_EQ(objects[0], results[0].id);
    ASSERT_EQ(new_object, results[1].id);
    ASSERT_EQ(additional, results[2].id);
    ASSERT_EQ(objects[1], results[3].id);
    ASSERT_EQ(objects[2], results[4].id);

    for (auto i = 1u; i < results.size(); ++i) {
        ASSERT_EQ(i, results[i - 1].sequence);
    }
}

TEST_F(DatabasePostObjectTest, AddPostObjectsDateModified) {
    const auto id = create_post();

    database.create_object(new_object, {}, {});
    database.create_post_objects(id, {new_object}, 0);

    const auto post = database.read_post(id);

    ASSERT_NE(post.date_created, post.date_modified);
}
