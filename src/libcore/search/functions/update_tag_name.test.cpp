#include "search.test.hpp"

using namespace std::literals;

TEST_F(SearchTagTest, UpdateName) {
    constexpr auto old_name = "JavaScript"sv;
    constexpr auto new_name = "Java"sv;

    auto names = std::vector<std::string>();

    netcore::run([&]() -> ext::task<> {
        const auto& tag = (co_await add_tag(old_name)).get();

        co_await search.update_tag_name(tag.id, old_name, new_name);

        const auto res = co_await get_tag(tag.id);
        names = res["names"].get<std::vector<std::string>>();
    }());

    const auto contains = [names](std::string_view name) -> bool {
        return std::find(names.begin(), names.end(), name) != names.end();
    };

    ASSERT_EQ(3, names.size());
    ASSERT_TRUE(contains(new_name));
    ASSERT_TRUE(contains("JS"));
    ASSERT_TRUE(contains("ECMAScript"));
}
