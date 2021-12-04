#include "database.test.h"

TEST_F(DatabasePostObjectTest, DeleteSingleObjectRange) {
    constexpr auto object = "38981dea-ff0c-46a5-a411-43d1e431803c";
    database.create_object(object, {}, {});

    const auto id = database.create_post("", "", {object}, {}).id;

    const auto range = minty::repo::db::range { .first = 0, .last = 0 };
    const auto ranges = std::vector<minty::repo::db::range> { range };
    database.delete_post_objects_ranges(id, ranges);

    const auto objects = database.read_post_objects(id);

    ASSERT_TRUE(objects.empty());
}

TEST_F(DatabasePostObjectTest, DeleteObjectRange) {
    const auto objects = std::vector<std::string> {
        "0e5bad49-9cf9-46e4-85c9-83ef9baf1f79",
        "adf85243-7673-467f-9fa6-56fc40642e06",
        "0797323f-910f-4f90-9bb1-dd145730c7a0"
    };

    for (const auto& object : objects) {
        database.create_object(object, {}, {});
    }

    const auto id = database.create_post("", "", objects, {}).id;

    const auto range = minty::repo::db::range { .first = 0, .last = 1 };
    const auto ranges = std::vector<minty::repo::db::range> { range };
    database.delete_post_objects_ranges(id, ranges);

    const auto result = database.read_post_objects(id);

    ASSERT_EQ(1, result.size());
    ASSERT_EQ(objects[2], result.front().id);
}

TEST_F(DatabasePostObjectTest, DeleteMultipleObjectsRange) {
    const auto objects = std::vector<std::string> {
        "0e5bad49-9cf9-46e4-85c9-83ef9baf1f79",
        "adf85243-7673-467f-9fa6-56fc40642e06",
        "0797323f-910f-4f90-9bb1-dd145730c7a0",
        "fc6478d2-2acb-4773-82c9-fbd4f756303d",
        "edfd4b08-334a-4969-8fe7-0acc15a77045"
    };

    for (const auto& object : objects) {
        database.create_object(object, {}, {});
    }

    const auto id = database.create_post("", "", objects, {}).id;

    const auto first = minty::repo::db::range { .first = 0, .last = 0 };
    const auto third = minty::repo::db::range { .first = 2, .last = 2 };
    const auto fifth = minty::repo::db::range { .first = 4, .last = 4 };
    const auto ranges = std::vector<minty::repo::db::range> {
        first,
        third,
        fifth
    };
    database.delete_post_objects_ranges(id, ranges);

    const auto result = database.read_post_objects(id);

    ASSERT_EQ(2, result.size());
    ASSERT_EQ(objects[1], result[0].id);
    ASSERT_EQ(objects[3], result[1].id);
}
