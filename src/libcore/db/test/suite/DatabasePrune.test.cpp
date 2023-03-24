#include "DatabasePrune.test.hpp"

auto DatabasePruneTest::create_site() -> ext::task<std::int64_t> {
    co_return (co_await db->create_site("https", "example.com", icon_id)).id;
}

auto DatabasePruneTest::prune_objects() -> ext::task<std::size_t> {
    const auto objects = co_await db->prune_objects();
    co_return objects.size();
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
