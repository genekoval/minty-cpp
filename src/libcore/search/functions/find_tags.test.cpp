#include "search.test.hpp"

using search_result = minty::search_result<UUID::uuid>;
using optional_tag =
    std::optional<std::reference_wrapper<const minty::core::db::tag_search>>;

using SearchTagFind = SearchTagTest;

// NOTE: Right now, the results are sorted by insertion order.

TEST_F(SearchTagFind, FindOne) {
    auto opt = optional_tag();
    auto result = search_result();

    netcore::run([&]() -> ext::task<> {
        opt = co_await add_tag("Python");

        co_await index.refresh();

        const auto query = minty::tag_query {.size = 10, .name = "Python"};

        result = co_await search.find_tags(query);
    }());

    const auto& tag = opt->get();

    ASSERT_EQ(1, result.total);
    ASSERT_EQ(result.total, result.hits.size());
    ASSERT_EQ(tag.id, result.hits.front());
}

TEST_F(SearchTagFind, FindMultiple) {
    auto java_opt = optional_tag();
    auto js_opt = optional_tag();
    auto result = search_result();

    netcore::run([&]() -> ext::task<> {
        java_opt = co_await add_tag("Java");
        js_opt = co_await add_tag("JavaScript");

        co_await index.refresh();

        const auto query = minty::tag_query {.size = 10, .name = "Java"};

        result = co_await search.find_tags(query);
    }());

    const auto& java = java_opt->get();
    const auto& js = js_opt->get();

    ASSERT_EQ(2, result.total);
    ASSERT_EQ(result.total, result.hits.size());
    EXPECT_EQ(java.id, result.hits.at(0));
    EXPECT_EQ(js.id, result.hits.at(1));
}

TEST_F(SearchTagFind, LimitResults) {
    auto opt = optional_tag();
    auto result = search_result();

    netcore::run([&]() -> ext::task<> {
        opt = co_await add_tag("Java");
        co_await add_tag("JavaScript");

        co_await index.refresh();

        const auto query = minty::tag_query {.size = 1, .name = "Java"};

        result = co_await search.find_tags(query);
    }());

    const auto& java = opt->get();

    ASSERT_EQ(2, result.total);
    ASSERT_EQ(1, result.hits.size());
    ASSERT_EQ(java.id, result.hits.at(0));
}

TEST_F(SearchTagFind, ResultWindow) {
    auto opt = optional_tag();
    auto result = search_result();

    netcore::run([&]() -> ext::task<> {
        co_await add_tag("Java");
        opt = co_await add_tag("JavaScript");

        co_await index.refresh();

        const auto query =
            minty::tag_query {.from = 1, .size = 1, .name = "Java"};

        result = co_await search.find_tags(query);
    }());

    const auto& js = opt->get();

    ASSERT_EQ(2, result.total);
    ASSERT_EQ(1, result.hits.size());
    ASSERT_EQ(js.id, result.hits.at(0));
}
