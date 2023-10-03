#pragma once

#include "Search.test.hpp"

namespace minty::test {
    auto now() -> time_point;

    auto parse_date(const std::string& string) -> time_point;
}

class SearchPostTest : public SearchTest {
protected:
    using post = minty::core::db::post_search;

    const UUID::uuid c = "2c78211d-f765-4dd8-b4d1-f3ea6b0d7b7f";
    const UUID::uuid cpp = "defa9484-99a8-437d-9c80-c6c3cb335cfb";
    const UUID::uuid java = "e60d2ea1-d791-4c55-a675-c6414251e361";
    const UUID::uuid js = "5a06b4bc-c244-456e-9f77-599151a20c13";
    const UUID::uuid rust = "9254ad34-4011-4a2f-bb7b-666b1a30f340";

    const UUID::uuid language = "41e6d051-fe15-4ff9-bd87-464f64c708e1";
    const UUID::uuid native = "4f7f5663-a81c-4cb9-a114-06c39a09a9df";
    const UUID::uuid jit = "c7b4c405-f4e4-421f-acb3-7de47e473584";

    const std::vector<post> posts = {
        {.id = cpp,
         .title = "C++",
         .description =
             "A general-purpose programming language created by Danish "
             "computer scientist Bjarne Stroustrup as an extension of the "
             "C programming language, or \"C with Classes\".",
         .visibility = minty::visibility::pub,
         .created = minty::test::parse_date("1985-01-01 12:00:00.0-04"),
         .modified = minty::test::parse_date("2020-12-15 12:00:00.0-04"),
         .tags = {language, native}},
        {.id = c,
         .title = "C",
         .description =
             "A successor to the programming language B, C was originally "
             "developed at Bell Labs by Dennis Ritchie between 1972 and "
             "1973 to construct utilities running on Unix.",
         .visibility = minty::visibility::pub,
         .created = minty::test::parse_date("1972-01-01 12:00:00.0-04"),
         .modified = minty::test::parse_date("2018-06-01 12:00:00.0-04"),
         .tags = {language, native}},
        {.id = java,
         .title = "Java",
         .description = "Java is a high-level, class-based, object-oriented "
                        "programming language that is designed to have as few "
                        "implementation dependencies as possible.",
         .visibility = minty::visibility::pub,
         .created = minty::test::parse_date("1995-05-23 12:00:00.0-04"),
         .modified = minty::test::parse_date("2022-03-22 12:00:00.0-04"),
         .tags = {language, jit}},
        {.id = js,
         .title = "JavaScript",
         .description =
             "A programming language that is one of the core technologies "
             "of the World Wide Web, alongside HTML and CSS.",
         .visibility = minty::visibility::pub,
         .created = minty::test::parse_date("1995-12-04 12:00:00.0-04"),
         .modified = minty::test::parse_date("2021-07-22 12:00:00.0-04"),
         .tags = {language, jit}},
        {.id = rust,
         .title = "Rust",
         .description =
             "Rust enforces memory safety — that is, that all references "
             "point to valid memory — without requiring the use of a "
             "garbage collector or reference counting present in other "
             "memory-safe languages.",
         .visibility = minty::visibility::pub,
         .created = minty::test::parse_date("2010-07-07 12:00:00.0-04"),
         .modified = minty::test::parse_date("2022-06-30 12:00:00.0-04"),
         .tags = {language, native}}};

    const UUID::uuid tag = "f14cdce9-33ae-4a52-8613-2761b44a9388";

    static auto SetUpTestSuite() -> void;

    minty::core::index& index = search.post_index;

    SearchPostTest();

    auto add_post() -> ext::task<std::reference_wrapper<const post>>;

    auto add_post(const UUID::uuid& id)
        -> ext::task<std::reference_wrapper<const post>>;

    auto find_post(const UUID::uuid& id) const -> const post&;

    auto get_post(const UUID::uuid& id) -> ext::task<elastic::json>;
};
