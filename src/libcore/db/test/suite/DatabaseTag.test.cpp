#include "DatabaseTag.test.hpp"

auto DatabaseTagTest::create_tag() -> UUID::uuid {
    return database.create_tag(tag_name);
}

auto DatabaseTagTest::tables() -> std::vector<std::string> {
    return {
        "tag",
        "site"
    };
}
