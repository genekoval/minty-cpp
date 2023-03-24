#include "DatabaseTag.test.hpp"

auto DatabaseTagTest::create_tag() -> ext::task<UUID::uuid> {
    co_return co_await db->create_tag(tag_name);
}

auto DatabaseTagTest::tables() -> std::vector<std::string> {
    return {
        "tag",
        "site"
    };
}
