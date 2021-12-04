#include "DatabasePrune.test.h"

auto DatabasePruneTest::create_site() -> std::string {
    return database.create_site(
        "https",
        "example.com",
        icon_id
    ).id;
}

auto DatabasePruneTest::prune_objects(bool commit) -> int {
    auto pruned = 0;
    database.prune_objects([&](auto objects) -> bool {
        pruned = objects.size();
        return commit;
    });

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
