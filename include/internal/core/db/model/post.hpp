#pragma once

#include "object_preview.hpp"
#include "visibility.hpp"

#include <minty/model/time_point.hpp>

#include <pg++/pg++>

namespace minty::core::db {
    struct post {
        UUID::uuid id;
        std::string title;
        std::string description;
        std::vector<object_preview> objects;
        std::int32_t comment_count;
        minty::visibility visibility;
        time_point date_created;
        time_point date_modified;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::post, "post");
