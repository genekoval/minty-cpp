#pragma once

#include "object_preview.hpp"

#include <minty/model/time_point.hpp>

#include <vector>

namespace minty::core::db {
    struct post_update {
        UUID::uuid id;
        std::optional<std::string> new_data;
        time_point date_modified;
    };

    struct post_object_update {
        std::vector<object_preview> objects;
        time_point date_modified;
    };
}
