#include "search.test.h"

using SearchTagAddAlias = SearchTagTest;

TEST_F(SearchTagAddAlias, Create) {
    auto tag_opt = std::optional<std::reference_wrapper<const tag>>();
    auto names = std::vector<std::string>();

    netcore::run([&]() -> ext::task<> {
        tag_opt = co_await add_tag();

        const auto& tag = tag_opt->get();

        auto res = co_await get_tag(tag.id);

        names = res["names"].get<std::vector<std::string>>();
    }());

    const auto& tag = tag_opt->get();

    ASSERT_EQ(1, names.size());
    ASSERT_EQ(tag.names.front(), names.front());
}

TEST_F(SearchTagAddAlias, Append) {
    constexpr auto name = "Foo Bar";

    auto tag_opt = std::optional<std::reference_wrapper<const tag>>();
    auto names = std::vector<std::string>();

    netcore::run([&]() -> ext::task<> {
        tag_opt = co_await add_tag();
        const auto& tag = tag_opt->get();

        co_await search.add_tag_alias(tag.id, name);

        auto res = co_await get_tag(tag.id);
        names = res["names"].get<std::vector<std::string>>();

    }());

    const auto& tag = tag_opt->get();

    ASSERT_EQ(2, names.size());
    EXPECT_EQ(tag.names.front(), names.at(0));
    EXPECT_EQ(name, names.at(1));
}

TEST_F(SearchTagAddAlias, Duplicate) {
    auto tag_opt = std::optional<std::reference_wrapper<const tag>>();
    auto names = std::vector<std::string>();

    netcore::run([&]() -> ext::task<> {
        tag_opt = co_await add_tag();
        const auto& tag = tag_opt->get();

        co_await search.add_tag_alias(tag.id, tag.names.front());

        auto res = co_await get_tag(tag.id);
        names = res["names"].get<std::vector<std::string>>();
    }());

    const auto& tag = tag_opt->get();

    ASSERT_EQ(1, names.size());
    EXPECT_EQ(tag.names.front(), names.front());
}
