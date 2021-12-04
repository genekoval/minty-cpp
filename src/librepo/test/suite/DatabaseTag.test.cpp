#include "DatabaseTag.test.h"

auto DatabaseTagTest::create_tag() -> std::string {
    return database.create_tag(tag_name);
}

auto DatabaseTagTest::tables() -> std::vector<std::string> {
    return {
        "tag",
        "site"
    };
}
