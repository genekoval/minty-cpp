#include "search.test.h"

using SearchTagDeleteAlias = SearchTagTest;

TEST_F(SearchTagDeleteAlias, Delete) {
    const auto& tag = add_tag("Rust");

    search.delete_tag_alias(tag.id, tag.names.front());

    auto res = get_tag(tag.id);
    const auto names = read_names(res);

    EXPECT_EQ(tag.names.size() - 1, names.size());

    for (auto i = 0ul; i < names.size(); ++i) {
        TIMBER_DEBUG("Checking tag name: \"{}\"", names.at(i));
        EXPECT_EQ(tag.names.at(i + 1), names.at(i)) << "at index " << i;
    }
}
