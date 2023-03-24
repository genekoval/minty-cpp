#pragma once

#include "source.hpp"

namespace minty::core::db {
    struct object {
        UUID::uuid id;
        std::optional<UUID::uuid> preview_id;
        std::optional<source> src;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::object, "object");
