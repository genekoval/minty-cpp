#include "search.test.h"

using namespace std::literals;

TEST_F(SearchTagTest, UpdateName) {
    constexpr auto old_name = "JavaScript"sv;
    constexpr auto new_name = "Java"sv;

    const auto& tag = add_tag(old_name);

    search.update_tag_name(tag.id, old_name, new_name);

    auto res = get_tag(tag.id);
    const auto names = read_names(res);

    const auto contains = [names](std::string_view name) -> bool {
        return std::find(names.begin(), names.end(), name) != names.end();
    };

    ASSERT_EQ(3, names.size());
    ASSERT_TRUE(contains(new_name));
    ASSERT_TRUE(contains("JS"));
    ASSERT_TRUE(contains("ECMAScript"));
}
