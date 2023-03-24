#include "database.test.hpp"

TEST_F(DatabaseTagTest, CreateTagSource) {
    constexpr auto resource = "/cats?v=100";

    run([&]() -> ext::task<> {
        const auto id = co_await create_tag();
        const auto site = co_await db->create_site(
            "https",
            "example.com",
            "937900e4-54a0-40fb-8ac8-315e5d3b2ae1"
        );

        const auto source = co_await db->create_source(site.id, resource);
        co_await db->create_tag_source(id, source.id);

        const auto sources = co_await db->read_tag_sources(id);

        EXPECT_EQ(1, sources.size());

        const auto& src = sources.at(0);

        EXPECT_EQ(resource, src.resource);
        EXPECT_EQ(site.id, src.website.id);
    }());
}
