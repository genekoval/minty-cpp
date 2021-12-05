#include "DatabasePost.test.h"

auto DatabasePostTest::create_post() -> std::string {
    return database.create_post("", "", {}, {}).id;
}

auto DatabasePostTest::tables() -> std::vector<std::string> {
    return {"post"};
}
