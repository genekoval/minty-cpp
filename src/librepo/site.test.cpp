#include "db.test.h"

#include <minty/error.h>

class DatabaseSiteTest : public DatabaseTest {
protected:
    auto tables() -> std::vector<std::string> override {
        return {"site"};
    }
};

TEST_F(DatabaseSiteTest, Create) {
    constexpr auto name = "wikipedia";
    constexpr auto homepage = "https://www.wikipedia.org/";
    constexpr auto thumbnail_id = "44eac3f9-2405-4f37-92e0-0e810c5ac4e8";

    const auto site = database.create_site(name, homepage, thumbnail_id);

    ASSERT_EQ(name, site.name);
    ASSERT_EQ(homepage, site.homepage);
    ASSERT_TRUE(site.thumbnail_id.has_value());
    ASSERT_EQ(thumbnail_id, site.thumbnail_id.value());
}

TEST_F(DatabaseSiteTest, CreateNoThumbnail) {
    constexpr auto name = "wikipedia";
    constexpr auto homepage = "https://www.wikipedia.org/";

    const auto site = database.create_site(name, homepage, {});

    ASSERT_EQ(name, site.name);
    ASSERT_EQ(homepage, site.homepage);
    ASSERT_FALSE(site.thumbnail_id.has_value());
}

TEST_F(DatabaseSiteTest, CreateDuplicateName) {
    const auto site = database.create_site(
        "wikipedia",
        "https://www.wikipedia.org",
        {}
    );

    try {
        database.create_site(
            site.name,
            "https://github.com",
            {}
        );
    }
    catch (const minty::unique_entity_violation& ex) {
        ASSERT_EQ(site.name, ex.value);
    }
}

TEST_F(DatabaseSiteTest, CreateDuplicateHomepage) {
    const auto site = database.create_site(
        "wikipedia",
        "https://www.wikipedia.org",
        {}
    );

    try {
        database.create_site(
            "github",
            site.homepage,
            {}
        );
    }
    catch (const minty::unique_entity_violation& ex) {
        ASSERT_EQ(site.homepage, ex.value);
    }
}
