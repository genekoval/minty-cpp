#include "DatabasePostObject.test.h"

auto DatabasePostObjectTest::SetUp() -> void {
    DatabasePostTest::SetUp();
    database.create_object(new_object, {}, {});
    post_id = create_post_with_objects();
}

auto DatabasePostObjectTest::create_post_with_objects() -> std::string {
    for (const auto& object : objects) {
        database.create_object(object, {}, {});
    }

    return database.create_post("", "", objects, {}).id;
}

auto DatabasePostObjectTest::insert_object(
    unsigned int position
) -> std::vector<minty::test::sequence_object> {
    return insert_objects({new_object}, position);
}

auto DatabasePostObjectTest::insert_objects(
    const std::vector<std::string>& objects,
    unsigned int position
) -> std::vector<minty::test::sequence_object> {
    database.create_post_objects(post_id, objects, position);
    return with_sequence();
}

auto DatabasePostObjectTest::tables() -> std::vector<std::string> {
    auto base = DatabasePostTest::tables();

    base.push_back("object");

    return base;
}

auto DatabasePostObjectTest::with_sequence()
-> std::vector<minty::test::sequence_object> {
    return exec<minty::test::sequence_object>(
        "SELECT object_id, sequence "
        "FROM data.post_object "
        "WHERE post_id = $1 "
        "ORDER BY sequence",
        post_id
    );
}
