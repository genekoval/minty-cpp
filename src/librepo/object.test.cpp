#include "db.test.h"

#include <minty/repo/db/model.h>

#include <timber/timber>
#include <typeinfo>

class DatabaseObjectTest : public DatabaseTest {
protected:
    static constexpr auto object_id = "0db27a78-b62f-41ff-9f6c-8e029b2a6342";
    static constexpr auto preview_id = "033d3aac-d27e-473c-b553-2d6b7aab9e88";
    static constexpr auto source_url = "https://example.com/hello.jpg";

    auto create_site() -> minty::repo::db::site {
        return database.create_site(
            "example",
            "https://example.com",
            "447a8641-fc39-4789-a785-96ef9ab3162d"
        );
    }

    auto tables() -> std::vector<std::string> override {
        return {
            "object",
            "source",
            "site",
            "object"
        };
    }
};

TEST_F(DatabaseObjectTest, CreateObject) {
    database.create_object(object_id, {}, {});
    ASSERT_EQ(1, count("object"));

    database.create_object(object_id, {}, {});
    ASSERT_EQ(1, count("object"));

    const auto object = database.read_object(object_id);

    ASSERT_EQ(object_id, object.id);
    ASSERT_FALSE(object.preview_id.has_value());
    ASSERT_FALSE(object.src.has_value());
}
