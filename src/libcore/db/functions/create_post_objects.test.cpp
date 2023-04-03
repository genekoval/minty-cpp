#include "database.test.hpp"

TEST_F(DatabasePostObjectTest, AppendPostObject) {
    run([&]() -> ext::task<> {
        const auto results = co_await insert_object(std::nullopt);

        EXPECT_EQ(4, results.size());

        for (auto i = 1u; i <= objects.size(); ++i) {
            const auto object = objects[i - 1];
            const auto result = results[i - 1];

            EXPECT_EQ(object, result.id);
            EXPECT_EQ(i, result.sequence);
        }

        EXPECT_EQ(new_object, results.back().id);
        EXPECT_EQ(4, results.back().sequence);
    }());
}

TEST_F(DatabasePostObjectTest, InsertPostObject) {
    auto results = std::vector<minty::test::sequence_object>();

    run([&]() -> ext::task<> {
        results = co_await insert_object(objects.at(1));
    }());

    ASSERT_EQ(4, results.size());

    EXPECT_EQ(objects[0], results[0].id);
    EXPECT_EQ(new_object, results[1].id);
    EXPECT_EQ(objects[1], results[2].id);
    EXPECT_EQ(objects[2], results[3].id);

    for (auto i = 1u; i < results.size(); ++i) {
        EXPECT_EQ(i, results[i - 1].sequence);
    }
}

TEST_F(DatabasePostObjectTest, InsertMultiplePostObjects) {
    constexpr auto additional = "c3563941-473d-4ed1-9ea8-ef9569642443";

    auto results = std::vector<minty::test::sequence_object>();

    run([&]() -> ext::task<> {
        co_await db->create_object(additional, {}, {});
        results = co_await insert_objects(
            {new_object, additional},
            objects.at(0)
        );
    }());

    ASSERT_EQ(5, results.size());

    EXPECT_EQ(new_object, results[0].id);
    EXPECT_EQ(additional, results[1].id);
    EXPECT_EQ(objects[0], results[2].id);
    EXPECT_EQ(objects[1], results[3].id);
    EXPECT_EQ(objects[2], results[4].id);

    for (auto i = 1u; i < results.size(); ++i) {
        EXPECT_EQ(i, results[i - 1].sequence);
    }
}

TEST_F(DatabasePostObjectTest, AddPostObjectsDateModified) {
    run([&]() -> ext::task<> {
        const auto id = co_await create_draft();

        co_await db->create_object(new_object, {}, {});
        co_await db->create_post_objects(id, {new_object}, std::nullopt);

        const auto post = co_await db->read_post(id);

        EXPECT_NE(post.date_created, post.date_modified);
    }());
}
