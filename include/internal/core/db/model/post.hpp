#pragma once

#include "visibility.hpp"

#include <minty/model/time_point.hpp>

#include <pg++/pg++>

namespace minty::core::db {
    struct post {
        UUID::uuid id;
        std::optional<std::string> title;
        std::optional<std::string> description;
        visibility visibility;
        time_point date_created;
        time_point date_modified;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::post, "post");
