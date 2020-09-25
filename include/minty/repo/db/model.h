#pragma once

#include <minty/model.h>

#include <pqxx/pqxx>

namespace minty::repo::db {
    struct tag : model::tag {
        tag(const pqxx::row& row);
    };
}
