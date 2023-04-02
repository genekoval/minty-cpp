#include "DatabasePostObject.test.hpp"

auto DatabasePostObjectTest::create_post_with_objects() ->
    ext::task<UUID::uuid>
{
    for (const auto& object : objects) {
        co_await db->create_object(object, {}, {});
    }

    const auto draft = co_await create_draft();
    co_await db->create_post_objects(draft, objects, -1);
    co_return draft;
}

auto DatabasePostObjectTest::insert_object(
    std::int16_t position
) -> ext::task<std::vector<minty::test::sequence_object>> {
    co_return co_await insert_objects({new_object}, position);
}

auto DatabasePostObjectTest::insert_objects(
    const std::vector<UUID::uuid>& objects,
    std::int16_t position
) -> ext::task<std::vector<minty::test::sequence_object>> {
    co_await db->create_post_objects(post_id, objects, position);
    co_return co_await with_sequence();
}

auto DatabasePostObjectTest::run(ext::task<>&& task) -> void {
    DatabasePostTest::run([this](ext::task<>&& task) -> ext::task<> {
        co_await db->create_object(new_object, {}, {});
        post_id = co_await create_post_with_objects();

        co_await std::forward<ext::task<>>(task);
    }(std::forward<ext::task<>>(task)));
}

auto DatabasePostObjectTest::tables() -> std::vector<std::string> {
    auto base = DatabasePostTest::tables();

    base.push_back("object");

    return base;
}

auto DatabasePostObjectTest::with_sequence() ->
    ext::task<std::vector<minty::test::sequence_object>>
{
    co_return co_await client->fetch_rows<minty::test::sequence_object>(
        "SELECT object_id, sequence "
        "FROM data.post_object "
        "WHERE post_id = $1 "
        "ORDER BY sequence",
        post_id
    );
}
