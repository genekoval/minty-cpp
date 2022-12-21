#include "database.test.hpp"

#include <minty/error.h>

TEST_F(DatabaseSiteTest, CreateSite) {
    constexpr auto scheme = "https";
    constexpr auto host = "wikipedia.org";
    constexpr auto icon = "44eac3f9-2405-4f37-92e0-0e810c5ac4e8";

    const auto site = database.create_site(scheme, host, icon);

    ASSERT_EQ(scheme, site.scheme);
    ASSERT_EQ(host, site.host);
    ASSERT_TRUE(site.icon.has_value());
    ASSERT_EQ(icon, site.icon.value());
}

TEST_F(DatabaseSiteTest, CreateSiteNoIcon) {
    constexpr auto scheme = "https";
    constexpr auto host = "wikipedia.org";

    const auto site = database.create_site(scheme, host, {});

    ASSERT_EQ(scheme, site.scheme);
    ASSERT_EQ(host, site.host);
    ASSERT_FALSE(site.icon.has_value());
}

TEST_F(DatabaseSiteTest, CreateSiteDuplicateName) {
    const auto site = database.create_site(
        "https",
        "wikipedia.org",
        {}
    );

    try {
        database.create_site(
            "http",
            site.host,
            {}
        );
    }
    catch (const minty::unique_entity_violation& ex) {
        ASSERT_EQ(site.host, ex.value);
    }
}
