#pragma once

#include "object_preview.hpp"

#include <minty/model/time_point.hpp>

namespace minty::core::db {
    struct post_preview {
        UUID::uuid id;
        std::optional<std::string> title;
        std::optional<object_preview> preview;
        std::uint32_t comment_count;
        std::uint32_t object_count;
        time_point date_created;
    };
}
