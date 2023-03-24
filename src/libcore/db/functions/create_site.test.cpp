#include "database.test.hpp"

#include <minty/except.hpp>

TEST_F(DatabaseSiteTest, CreateSite) {
    constexpr auto scheme = "https";
    constexpr auto host = "wikipedia.org";
    constexpr auto icon = "44eac3f9-2405-4f37-92e0-0e810c5ac4e8";

    run([&]() -> ext::task<> {
        const auto site = co_await db->create_site(scheme, host, icon);

        EXPECT_EQ(scheme, site.scheme);
        EXPECT_EQ(host, site.host);
        EXPECT_TRUE(site.icon.has_value());
        EXPECT_EQ(icon, site.icon.value());
    }());
}

TEST_F(DatabaseSiteTest, CreateSiteNoIcon) {
    constexpr auto scheme = "https";
    constexpr auto host = "wikipedia.org";

    run([&]() -> ext::task<> {
        const auto site = co_await db->create_site(scheme, host, {});

        EXPECT_EQ(scheme, site.scheme);
        EXPECT_EQ(host, site.host);
        EXPECT_FALSE(site.icon.has_value());
    }());
}
