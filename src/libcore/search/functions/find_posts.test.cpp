#include "search.test.h"

using minty::core::post_query;
using minty::core::sort_order;
using sort_value = minty::core::post_sort_value;

constexpr auto asc = sort_order::ascending;
constexpr auto desc = sort_order::descending;

constexpr auto created = sort_value::date_created;
constexpr auto modified = sort_value::date_modified;
constexpr auto relevance = sort_value::relevance;
constexpr auto title = sort_value::title;

class SearchPostFind : public SearchPostTest {
protected:
    virtual auto SetUp() -> void override {
        SearchPostTest::SetUp();

        const auto errors = search.add_posts(posts);

        for (const auto& error : errors) TIMBER_ERROR(error);

        ASSERT_TRUE(errors.empty());

        index.refresh();
    }

    auto find(
        post_query&& query,
        const std::vector<UUID::uuid>& expected
    ) -> void {
        find(std::move(query), expected, expected.size());
    }

    auto find(
        post_query&& query,
        const std::vector<UUID::uuid>& expected,
        unsigned int total
    ) -> void {
        if (query.size == 0) query.size = expected.size();

        const auto result = search.find_posts(query);

        ASSERT_EQ(expected.size(), result.hits.size());
        ASSERT_EQ(total, result.total);

        const auto size = expected.size();

        for (auto i = 0ul; i < size; ++i) {
            EXPECT_EQ(expected[i], result.hits[i]) << "at index " << i;
        }
    }
};

TEST_F(SearchPostFind, LimitResults) {
    find({
        .size = 3,
        .sort = {
            .value = title,
            .order = asc
        }
    }, {
        c,
        cpp,
        java
    },
        posts.size()
    );
}

TEST_F(SearchPostFind, SortCreatedAscending) {
    find({
        .sort = {
            .value = created,
            .order = asc
        }
    }, {
        c,
        cpp,
        java,
        js,
        rust
    });
}

TEST_F(SearchPostFind, SortCreatedDescending) {
    find({
        .sort = {
            .value = created,
            .order = desc
        }
    }, {
        rust,
        js,
        java,
        cpp,
        c
    });
}

TEST_F(SearchPostFind, SortModifiedAscending) {
    find({
        .sort = {
            .value = modified,
            .order = asc
        }
    }, {
        c,
        cpp,
        js,
        java,
        rust
    });
}

TEST_F(SearchPostFind, SortModifiedDescending) {
    find({
        .sort = {
            .value = modified,
            .order = desc
        }
    }, {
        rust,
        java,
        js,
        cpp,
        c
    });
}


TEST_F(SearchPostFind, SortTitleAscending) {
    find({
        .sort = {
            .value = title,
            .order = asc
        },
    }, {
        c,
        cpp,
        java,
        js,
        rust
    });
}

TEST_F(SearchPostFind, SortTitleDescending) {
    find({
        .sort = {
            .value = title,
            .order = desc
        },
    }, {
        rust,
        js,
        java,
        cpp,
        c
    });
}

TEST_F(SearchPostFind, Tag) {
    find({
        .tags = {
            language
        },
        .sort = {
            .value = title,
            .order = asc
        }
    }, {
        c,
        cpp,
        java,
        js,
        rust
    });

    find({
        .tags = {
            native
        },
        .sort = {
            .value = title,
            .order = asc
        }
    }, {
        c,
        cpp,
        rust
    });
}

TEST_F(SearchPostFind, TextDescription) {
    find({
        .text = "programming language",
        .sort = {
            .value = title,
            .order = asc
        }
    }, {
        c,
        cpp,
        java,
        js
    });

    find({
        .text = "html"
    }, {
        js
    });
}

TEST_F(SearchPostFind, TextTitle) {
    find({
        .text = "java"
    }, {
        java
    });

    find({
        .text = "c",
        .sort = {
            .value = title,
            .order = asc
        }
    }, {
        c,
        cpp
    });
}
