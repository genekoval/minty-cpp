#pragma once

#include <uuid++/uuid++>

namespace minty::core::db {
    struct object_preview {
        UUID::uuid id;
        std::optional<UUID::uuid> preview_id;
    };
}
