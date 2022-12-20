#pragma once

#include <minty/model/time_point.hpp>

#include <uuid++/uuid++>
#include <vector>

namespace minty::repo::db {
    struct tag {
        UUID::uuid id;
        std::string name;
        std::vector<std::string> aliases;
        std::optional<std::string> description;
        std::optional<UUID::uuid> avatar;
        std::optional<UUID::uuid> banner;
        std::uint32_t post_count;
        time_point date_created;
    };
}
