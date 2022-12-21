#pragma once

#include <uuid++/uuid++>
#include <vector>

namespace minty::core::db {
    struct tag_search {
        UUID::uuid id;
        std::vector<std::string> names;
    };

}
