#include "SearchTag.test.hpp"

#include <minty/except.hpp>

SearchTagTest::SearchTagTest() {
    netcore::run([&]() -> ext::task<> {
        co_await index.refresh();
        co_await index.clear();
    }());
}

auto SearchTagTest::SetUpTestSuite() -> void {
    netcore::run([]() -> ext::task<> {
        auto& search = minty::test::SearchEnvironment::engine();
        auto& index = search.*index_member;

        if (!co_await index.exists()) co_await index.create();
    }());
}

auto SearchTagTest::add_tag() -> ext::task<std::reference_wrapper<const tag>> {
    const auto& tag = tags.front();

    co_await search.add_tag_alias(tag.id, tag.names.front());

    co_return tag;
}

auto SearchTagTest::add_tag(
    std::string_view name
) -> ext::task<std::reference_wrapper<const tag>> {
    const auto result = std::find_if(
        tags.begin(),
        tags.end(),
        [name](const auto& tag) {
            return std::find(
                tag.names.begin(),
                tag.names.end(),
                name
            ) != tag.names.end();
        }
    );

    if (result == tags.end()) {
        throw minty::minty_error("no tag with name '{}'", name);
    }

    const auto errors = co_await search.add_tags(
        std::span<const tag>(result, result + 1)
    );

    if (!errors.empty()) throw minty::minty_error(errors.front());

    const auto& tag = *result;

    TIMBER_DEBUG("Found tag with name '{}': {}", name, tag.id);

    co_return tag;
}

auto SearchTagTest::get_tag(const UUID::uuid& id) -> ext::task<elastic::json> {
    co_return co_await client
        .get_doc_source(index, id)
        .send();
}
