#include "database.test.hpp"

TEST_F(DatabaseTagTest, CreateTagSource) {
    constexpr auto resource = "/cats?v=100";

    const auto id = create_tag();
    const auto site = database.create_site(
        "https",
        "example.com",
        "937900e4-54a0-40fb-8ac8-315e5d3b2ae1"
    );

    const auto source = database.create_source(site.id, resource);
    database.create_tag_source(id, source.id);

    const auto sources = database.read_tag_sources(id);

    ASSERT_EQ(1, sources.size());

    const auto& src = sources.front();

    ASSERT_EQ(resource, src.resource);
    ASSERT_EQ(site.id, src.website.id);
}
