#include "DatabasePostObject.test.hpp"

auto DatabasePostObjectTest::create_post_with_objects() ->
    ext::task<UUID::uuid>
{
    for (const auto& object : objects) {
        co_await db->create_object(object, {}, {});
    }

    const auto draft = co_await create_draft();
    co_await db->create_post_objects(draft, objects, std::nullopt);
    co_return draft;
}

auto DatabasePostObjectTest::insert_object(
    const std::optional<UUID::uuid>& destination
) -> ext::task<std::vector<minty::core::db::object_preview>> {
    co_return co_await insert_objects({new_object}, destination);
}

auto DatabasePostObjectTest::insert_objects(
    const std::vector<UUID::uuid>& objects,
    const std::optional<UUID::uuid>& destination
) -> ext::task<std::vector<minty::core::db::object_preview>> {
    co_await db->create_post_objects(post_id, objects, destination);
    const auto post = co_await db->read_post(post_id);
    co_return post.objects;
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
