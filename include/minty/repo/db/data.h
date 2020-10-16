#pragma once

#include <minty/repo/db/model.h>

namespace minty::repo::db::data {
    struct source {
        const site* website;
        std::string_view url;
    };
};
