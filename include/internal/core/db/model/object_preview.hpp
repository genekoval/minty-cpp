#pragma once

#include <pg++/pg++>

namespace minty::core::db {
    struct object_preview {
        UUID::uuid id;
        std::optional<UUID::uuid> preview_id;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::object_preview, "object_preview");
