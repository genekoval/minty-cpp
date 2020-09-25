#include "repo.h"

#include <minty/repo/db/model.h>

namespace minty::repo::db {
    tag::tag(const pqxx::row& row) : model::tag {
        parse_field<0, decltype(id)>(row),
        parse_field<1, decltype(name)>(row),
        parse_field<2, decltype(color)>(row),
        parse_field<3, decltype(date_created)>(row),
    }
    {}
}
