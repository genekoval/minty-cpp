#include "search.test.h"

using SearchTagAddAlias = SearchTagTest;

TEST_F(SearchTagAddAlias, Create) {
    const auto& tag = add_tag();

    auto res = get_tag(tag.id);

    const auto names = res["names"].get<std::vector<std::string>>();

    ASSERT_EQ(1, names.size());
    ASSERT_EQ(tag.names.front(), names.front());
}

TEST_F(SearchTagAddAlias, Append) {
    constexpr auto name = "Foo Bar";

    const auto& tag = add_tag();

    search.add_tag_alias(tag.id, name);

    auto res = get_tag(tag.id);

    const auto names = res["names"].get<std::vector<std::string>>();

    ASSERT_EQ(2, names.size());
    ASSERT_EQ(tag.names.front(), names.at(0));
    ASSERT_EQ(name, names.at(1));
}

TEST_F(SearchTagAddAlias, Duplicate) {
    const auto& tag = add_tag();

    search.add_tag_alias(tag.id, tag.names.front());

    auto res = get_tag(tag.id);

    const auto names = res["names"].get<std::vector<std::string>>();

    ASSERT_EQ(1, names.size());
    ASSERT_EQ(tag.names.front(), names.front());
}
