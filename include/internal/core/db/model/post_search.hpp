#pragma once

#include "visibility.hpp"

#include <minty/model/time_point.hpp>

#include <ext/json.hpp>
#include <pg++/pg++>
#include <uuid++/json.hpp>

namespace minty::core::db {
    struct post_search {
        UUID::uuid id;
        std::optional<std::string> title;
        std::optional<std::string> description;
        minty::visibility visibility;
        time_point created;
        time_point modified;
        std::vector<UUID::uuid> tags;

        auto operator==(const post_search&) const -> bool = default;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        post_search,
        title,
        description,
        visibility,
        created,
        modified,
        tags
    )
}

PGCPP_COMPOSITE_DECL(minty::core::db::post_search, "post_search");
