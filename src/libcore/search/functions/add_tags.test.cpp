#include "../json/json.h"
#include "search.test.h"

using SearchTagAddTags = SearchTagTest;

TEST_F(SearchTagAddTags, Add) {
    const auto errors = search.add_tags(tags);
    EXPECT_TRUE(errors.empty());

    index.refresh();

    auto res = client
        .search(index, json({
            {"size", tags.size()}
        }).dump())
        .send();

    const auto total = uint64_t(res["hits"]["total"]["value"]);

    auto hits = std::vector<std::string_view>();
    for (auto hit : res["hits"]["hits"]) hits.emplace_back(hit["_id"]);

    EXPECT_EQ(tags.size(), total);
    EXPECT_EQ(total, hits.size());

    for (const auto& tag : tags) {
        const auto& id = tag.id;
        const auto end = hits.end();
        const auto contains = std::find(hits.begin(), end, id) != end;

        EXPECT_TRUE(contains) << "result does not contain tag [" << id << "]";
    }
}

TEST_F(SearchTagAddTags, Errors) {
    auto errors = search.add_tags(tags);
    EXPECT_TRUE(errors.empty());

    index.refresh();

    constexpr auto duplicates = 3;

    errors = search.add_tags(std::span<const tag>(
        tags.begin(),
        tags.begin() + duplicates
    ));

    EXPECT_EQ(duplicates, errors.size());
}
