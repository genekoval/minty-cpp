#pragma once

#include <uuid++/uuid++>

namespace minty::core::db {
    struct site {
        std::string id;
        std::string scheme;
        std::string host;
        std::optional<UUID::uuid> icon;
    };
}
