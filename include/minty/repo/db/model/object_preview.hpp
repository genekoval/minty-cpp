#pragma once

#include <uuid++/uuid++>

namespace minty::repo::db {
    struct object_preview {
        UUID::uuid id;
        std::optional<UUID::uuid> preview_id;
    };
}
