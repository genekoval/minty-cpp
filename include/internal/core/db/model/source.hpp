#pragma once

#include "site.hpp"

#include <minty/model/source.hpp>

namespace minty::core::db {
    struct source {
        std::int64_t id;
        std::string resource;
        site website;

        operator minty::source() const;
    };
}

PGCPP_COMPOSITE_DECL(minty::core::db::source, "source");
