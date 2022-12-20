#pragma once

#include "site.hpp"

#include <minty/model/source.hpp>

namespace minty::repo::db {
    struct source {
        std::string id;
        std::string resource;
        site website;

        operator minty::source() const;
    };
}
