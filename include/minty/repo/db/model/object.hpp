#pragma once

#include "source.hpp"

namespace minty::repo::db {
    struct object {
        UUID::uuid id;
        std::optional<UUID::uuid> preview_id;
        std::optional<source> src;
    };
}
