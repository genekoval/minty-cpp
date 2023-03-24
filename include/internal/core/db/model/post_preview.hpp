#pragma once

#include "object_preview.hpp"

#include <minty/model/time_point.hpp>

namespace minty::core::db {
    struct post_preview {
        UUID::uuid id;
        std::optional<std::string> title;
        std::optional<object_preview> preview;
        std::int32_t comment_count;
        std::int32_t object_count;
        time_point date_created;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::post_preview, "post_preview");
