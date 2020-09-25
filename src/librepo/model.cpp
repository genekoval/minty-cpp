#include "repo.h"

#include <minty/repo/db/model.h>

namespace minty::repo::db {
    tag::tag(const pqxx::row& row) : model::tag {
        parse_field<0, decltype(name)>(row),
        parse_field<1, decltype(color)>(row),
        parse_field<2, decltype(date_created)>(row),
    }
    {}
}
