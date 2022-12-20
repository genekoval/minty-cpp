#pragma once

#include <uuid++/uuid++>

namespace minty::repo::db {
    struct site {
        std::string id;
        std::string scheme;
        std::string host;
        std::optional<UUID::uuid> icon;
    };
}
