#pragma once

#include <minty/model/time_point.hpp>

#include <pg++/pg++>

namespace minty::core::db {
    struct post_search {
        UUID::uuid id;
        std::optional<std::string> title;
        std::optional<std::string> description;
        time_point created;
        time_point modified;
        std::vector<UUID::uuid> tags;

        auto operator==(const post_search&) const -> bool = default;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::post_search, "post_search");
