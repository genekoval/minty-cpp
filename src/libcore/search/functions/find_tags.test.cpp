#include "search.test.h"

using SearchTagFind = SearchTagTest;

// NOTE: Right now, the results are sorted by insertion order.

TEST_F(SearchTagFind, FindOne) {
    const auto& tag = add_tag("Python");

    index.refresh();

    const auto result = search.find_tags({
        .size = 10,
        .name = "Python"
    });

    ASSERT_EQ(1, result.total);
    ASSERT_EQ(result.total, result.hits.size());
    ASSERT_EQ(tag.id, result.hits.front());
}

TEST_F(SearchTagFind, FindMultiple) {
    const auto& java = add_tag("Java");
    const auto& js = add_tag("JavaScript");

    index.refresh();

    const auto result = search.find_tags({
        .size = 10,
        .name = "Java"
    });

    ASSERT_EQ(2, result.total);
    ASSERT_EQ(result.total, result.hits.size());
    EXPECT_EQ(java.id, result.hits.at(0));
    EXPECT_EQ(js.id, result.hits.at(1));
}

TEST_F(SearchTagFind, LimitResults) {
    const auto& java = add_tag("Java");
    add_tag("JavaScript");

    index.refresh();

    const auto result = search.find_tags({
        .size = 1,
        .name = "Java"
    });

    ASSERT_EQ(2, result.total);
    ASSERT_EQ(1, result.hits.size());
    ASSERT_EQ(java.id, result.hits.at(0));
}

TEST_F(SearchTagFind, ResultWindow) {
    add_tag("Java");
    const auto& js = add_tag("JavaScript");

    index.refresh();

    const auto result = search.find_tags({
        .from = 1,
        .size = 1,
        .name = "Java"
    });

    ASSERT_EQ(2, result.total);
    ASSERT_EQ(1, result.hits.size());
    ASSERT_EQ(js.id, result.hits.at(0));
}
