#pragma once

#include "Search.test.hpp"

class SearchTagTest : public SearchTest {
    static constexpr auto index_member = &minty::core::search_engine::tag_index;
protected:
    using tag = minty::repo::db::tag_search;

    static auto SetUpTestSuite() -> void;

    const std::vector<tag> tags = {
        {
            .id = "490574c7-00c2-463f-a484-52cbb999bfb2",
            .names = {
                "Python"
            }
        },
        {
            .id = "b3bce7ff-c059-41f9-a138-b4b246b7d620",
            .names = {
                "JavaScript",
                "JS",
                "ECMAScript"
            }
        },
        {
            .id = "03b0c82d-2c9f-442d-8454-a4a6ca5a47f8",
            .names = {
                "C++",
                "C with Classes"
            }
        },
        {
            .id = "f3a1a614-4e8b-4c05-ad42-7c1879db94da",
            .names = {
                "Rust",
                "Ferris",
                "rust-lang"
            }
        },
        {
            .id = "bf665440-7ead-4810-a5df-434da25cf8c1",
            .names = {
                "Go",
                "Golang",
                "Gopher"
            }
        },
        {
            .id = "8212a2d5-89b6-442d-9428-dae306438bac",
            .names = {
                "Java"
            }
        }
    };

    minty::core::index& index = search.*index_member;

    SearchTagTest();

    auto add_tag() -> ext::task<std::reference_wrapper<const tag>>;

    auto add_tag(
        std::string_view name
    ) -> ext::task<std::reference_wrapper<const tag>>;

    auto get_tag(const UUID::uuid& id) -> ext::task<elastic::json>;
};
