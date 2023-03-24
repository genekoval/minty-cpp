#pragma once

#include <pg++/pg++>

namespace minty::core::db {
    struct site {
        std::int64_t id;
        std::string scheme;
        std::string host;
        std::optional<UUID::uuid> icon;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::site, "site");
