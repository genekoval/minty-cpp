#include "SearchTag.test.h"

#include <minty/error.h>

SearchTagTest::SearchTagTest() {
    index.refresh();
    index.clear();
}

auto SearchTagTest::SetUpTestSuite() -> void {
    auto& search = minty::test::SearchEnvironment::engine();
    auto& index = search.*index_member;

    if (!index.exists()) index.create();
}

auto SearchTagTest::add_tag() -> const tag& {
    const auto& tag = tags.front();

    search.add_tag_alias(tag.id, tag.names.front());

    return tag;
}

auto SearchTagTest::get_tag(const UUID::uuid& id) -> elastic::json {
    return client
        .get_doc_source(index, id)
        .send();
}

auto SearchTagTest::add_tag(std::string_view name) -> const tag& {
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

    const auto errors = search.add_tags(
        std::span<const tag>(result, result + 1)
    );

    if (!errors.empty()) throw minty::minty_error(errors.front());

    const auto& tag = *result;

    TIMBER_DEBUG("Found tag with name '{}': {}", name, tag.id);

    return tag;
}
