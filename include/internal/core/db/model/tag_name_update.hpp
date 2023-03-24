#pragma once

#include "tag_name.hpp"

#include <pg++/pg++>

namespace minty::core::db {
    struct tag_name_update {
        tag_name names;
        std::optional<std::string> old_name;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::tag_name_update, "tag_name_update");
