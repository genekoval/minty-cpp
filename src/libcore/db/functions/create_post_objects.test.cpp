#include "database.test.hpp"

using minty::core::db::object_preview;

TEST_F(DatabasePostObjectTest, AppendPostObject) {
    run([&]() -> ext::task<> {
        const auto results = co_await insert_object(std::nullopt);

        EXPECT_EQ(4, results.size());

        for (auto i = 1u; i <= objects.size(); ++i) {
            const auto object = objects.at(i - 1);
            const auto result = results.at(i - 1);

            EXPECT_EQ(object, result.id);
        }

        EXPECT_EQ(new_object, results.back().id);
    }());
}

TEST_F(DatabasePostObjectTest, InsertPostObject) {
    auto results = std::vector<object_preview>();

    run([&]() -> ext::task<> {
        results = co_await insert_object(objects.at(1));
    }());

    ASSERT_EQ(4, results.size());

    EXPECT_EQ(objects.at(0), results.at(0).id);
    EXPECT_EQ(new_object, results.at(1).id);
    EXPECT_EQ(objects.at(1), results.at(2).id);
    EXPECT_EQ(objects.at(2), results.at(3).id);
}

TEST_F(DatabasePostObjectTest, InsertMultiplePostObjects) {
    constexpr auto additional = "c3563941-473d-4ed1-9ea8-ef9569642443";

    auto results = std::vector<object_preview>();

    run([&]() -> ext::task<> {
        co_await db->create_object(additional, {}, {});
        results = co_await insert_objects(
            {new_object, additional},
            objects.at(0)
        );
    }());

    ASSERT_EQ(5, results.size());

    EXPECT_EQ(new_object, results.at(0).id);
    EXPECT_EQ(additional, results.at(1).id);
    EXPECT_EQ(objects.at(0), results.at(2).id);
    EXPECT_EQ(objects.at(1), results.at(3).id);
    EXPECT_EQ(objects.at(2), results.at(4).id);
}

TEST_F(DatabasePostObjectTest, MoveObjects) {
    auto results = std::vector<object_preview>();

    run([&]() -> ext::task<> {
        results = co_await insert_objects(
            {objects.at(1), objects.at(2)},
            objects.at(0)
        );
    }());

    ASSERT_EQ(3, results.size());

    EXPECT_EQ(objects.at(1), results.at(0).id);
    EXPECT_EQ(objects.at(2), results.at(1).id);
    EXPECT_EQ(objects.at(0), results.at(2).id);
}

TEST_F(DatabasePostObjectTest, InsertDuplicates) {
    auto results = std::vector<object_preview>();

    run([&]() -> ext::task<> {
        results = co_await insert_objects(
            {
                objects.at(0),
                objects.at(2),
                objects.at(0),
                objects.at(0),
                objects.at(2)
            },
            std::nullopt
        );
    }());

    ASSERT_EQ(objects.size(), results.size());

    EXPECT_EQ(objects.at(1), results.at(0).id);
    EXPECT_EQ(objects.at(0), results.at(1).id);
    EXPECT_EQ(objects.at(2), results.at(2).id);
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
