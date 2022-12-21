#include "search.test.hpp"

using SearchTagDeleteAlias = SearchTagTest;

TEST_F(SearchTagDeleteAlias, Delete) {
    netcore::run([&]() -> ext::task<> {
        const auto& tag = (co_await add_tag("Rust")).get();

        co_await search.delete_tag_alias(tag.id, tag.names.front());

        const auto result = co_await get_tag(tag.id);
        const auto names = result["names"].get<std::vector<std::string>>();

        EXPECT_EQ(tag.names.size() - 1, names.size());

        for (auto i = 0ul; i < names.size(); ++i) {
            TIMBER_DEBUG("Checking tag name: \"{}\"", names.at(i));
            EXPECT_EQ(tag.names.at(i + 1), names.at(i)) << "at index " << i;
        }
    }());
}
