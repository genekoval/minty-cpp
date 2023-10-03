#include "search.test.hpp"

using nlohmann::json;

using SearchTagAddTags = SearchTagTest;

TEST_F(SearchTagAddTags, Add) {
    netcore::run([&]() -> ext::task<> {
        const auto errors = co_await search.add_tags(tags);
        EXPECT_TRUE(errors.empty());

        co_await index.refresh();

        const auto query = json({{"size", tags.size()}}).dump();
        auto req = client.search(index, query);
        const auto res = co_await req.send();

        const auto total = res["hits"]["total"]["value"].get<unsigned int>();

        auto hits = std::vector<UUID::uuid>();
        for (auto hit : res["hits"]["hits"]) {
            hits.push_back(hit["_id"].get<UUID::uuid>());
        }

        EXPECT_EQ(tags.size(), total);
        EXPECT_EQ(total, hits.size());

        for (const auto& tag : tags) {
            const auto& id = tag.id;
            const auto end = hits.end();
            const auto contains = std::find(hits.begin(), end, id) != end;

            EXPECT_TRUE(contains)
                << "result does not contain tag [" << id << "]";
        }
    }());
}

TEST_F(SearchTagAddTags, Errors) {
    netcore::run([&]() -> ext::task<> {
        auto errors = co_await search.add_tags(tags);
        EXPECT_TRUE(errors.empty());

        co_await index.refresh();

        constexpr auto duplicates = 3;

        errors = co_await search.add_tags(
            std::span<const tag>(tags.begin(), tags.begin() + duplicates)
        );

        EXPECT_EQ(duplicates, errors.size());
    }());
}
