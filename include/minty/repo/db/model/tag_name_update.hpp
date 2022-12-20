#pragma once

#include <minty/model/tag_name.hpp>

namespace minty::repo::db {
    struct tag_name_update {
        tag_name names;
        std::optional<std::string> old_name;
    };
}
