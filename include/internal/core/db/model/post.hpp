#pragma once

#include <minty/model/time_point.hpp>

#include <uuid++/uuid++>

namespace minty::core::db {
    struct post {
        UUID::uuid id;
        std::optional<std::string> title;
        std::optional<std::string> description;
        time_point date_created;
        time_point date_modified;
    };
}
