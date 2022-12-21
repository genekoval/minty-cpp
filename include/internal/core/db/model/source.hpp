#pragma once

#include "site.hpp"

#include <minty/model/source.hpp>

namespace minty::core::db {
    struct source {
        std::string id;
        std::string resource;
        site website;

        operator minty::source() const;
    };
}
