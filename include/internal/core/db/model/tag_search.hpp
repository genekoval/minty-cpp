#pragma once

#include <pg++/pg++>

namespace minty::core::db {
    struct tag_search {
        UUID::uuid id;
        std::vector<std::string> names;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::tag_search, "tag_search");
