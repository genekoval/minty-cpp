#include "DatabasePost.test.h"

auto DatabasePostTest::create_post() -> std::string {
    return database.create_post("", "", {}, {}).id;
}

auto DatabasePostTest::create_post_with_objects() -> std::string {
    for (const auto& object : objects) {
        database.create_object(object, {}, {});
    }

    return database.create_post("", "", objects, {}).id;
}

auto DatabasePostTest::tables() -> std::vector<std::string> {
    return {"post"};
}
