#include "DatabasePrune.test.hpp"

auto DatabasePruneTest::create_site() -> std::string {
    return database.create_site(
        "https",
        "example.com",
        icon_id
    ).id;
}

auto DatabasePruneTest::prune_objects(bool commit) -> int {
    auto pruned = 0;

    [&]() -> ext::detached_task {
        co_await database.prune_objects([&](auto objects) -> ext::task<bool> {
            pruned = objects.size();
            co_return commit;
        });
    }();

    return pruned;
}

auto DatabasePruneTest::tables() -> std::vector<std::string> {
    return {
        "object",
        "object_ref",
        "post",
        "site",
        "source"
    };
}
