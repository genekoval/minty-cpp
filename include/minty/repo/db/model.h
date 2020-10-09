#pragma once

#include <minty/model.h>

#include <pqxx/pqxx>

namespace minty::repo::db {
    struct site : model::site {
        static constexpr auto entity = "site";

        site(const pqxx::row& row);
    };

    struct tag : model::tag {
        static constexpr auto entity = "tag";

        tag(const pqxx::row& row);
    };
}
