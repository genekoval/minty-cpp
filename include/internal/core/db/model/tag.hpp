#pragma once

#include <minty/model/time_point.hpp>

#include <pg++/pg++>

namespace minty::core::db {
    struct tag {
        UUID::uuid id;
        std::string name;
        std::vector<std::string> aliases;
        std::optional<std::string> description;
        std::optional<UUID::uuid> avatar;
        std::optional<UUID::uuid> banner;
        std::int32_t post_count;
        time_point date_created;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::tag, "tag");
