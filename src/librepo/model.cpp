#include "db.h"

#include <minty/repo/db/model.h>

namespace minty::repo::db {
    site::site(const pqxx::row& row) : model::site {
        parse_field<decltype(id)>(row, 0),
        parse_field<decltype(name)>(row, 1),
        parse_field<decltype(homepage)>(row, 2),
        parse_field<decltype(thumbnail_id)>(row, 3)
    }
    {}

    tag::tag(const pqxx::row& row) : model::tag {
        parse_field<decltype(id)>(row, 0),
        parse_field<decltype(name)>(row, 1),
        parse_field<decltype(color)>(row, 2),
        parse_field<decltype(date_created)>(row, 3),
    }
    {}
}
