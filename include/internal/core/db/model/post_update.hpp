#pragma once

#include "object_preview.hpp"

#include <minty/model/time_point.hpp>

namespace minty::core::db {
    struct post_update {
        UUID::uuid id;
        std::optional<std::string> new_data;
        time_point date_modified;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::post_update, "post_update");
