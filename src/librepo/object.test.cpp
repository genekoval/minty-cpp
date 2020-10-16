#include "db.test.h"

class DatabaseObjectTest : public DatabaseTest {
protected:
    auto tables() -> std::vector<std::string> override {
        return {
            "object",
            "site",
            "source"
        };
    }
};

TEST_F(DatabaseObjectTest, Add) {
    constexpr auto object_id = "0db27a78-b62f-41ff-9f6c-8e029b2a6342";
    constexpr auto preview_id = "033d3aac-d27e-473c-b553-2d6b7aab9e88";
    constexpr auto source_url = "https://example.com/hello.jpg";

    const auto site = database.create_site(
        "example",
        "https://example.com",
        "447a8641-fc39-4789-a785-96ef9ab3162d"
    );

    const auto src_data = minty::repo::db::data::source {
        .website = &site,
        .url = source_url
    };

    const auto object = database.add_object(
        object_id,
        preview_id,
        src_data
    );

    ASSERT_EQ(object_id, object.id);
    ASSERT_EQ(preview_id, object.preview_id);

    ASSERT_TRUE(object.src.has_value());
    const auto& source = object.src.value();

    ASSERT_EQ(source_url, source.url);
    ASSERT_EQ(site.id, source.website.id);
    ASSERT_EQ(site.homepage, source.website.homepage);
    ASSERT_EQ(site.thumbnail_id, source.website.thumbnail_id);
}
