#include "DatabaseObject.test.hpp"

auto DatabaseObjectTest::create_site() -> minty::core::db::site {
    return database.create_site(
        "example",
        "https://example.com",
        "447a8641-fc39-4789-a785-96ef9ab3162d"
    );
}

auto DatabaseObjectTest::tables() -> std::vector<std::string> {
    return {
        "object",
        "source",
        "site",
        "object"
    };
}
