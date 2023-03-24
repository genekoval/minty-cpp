#include <internal/core/db/model/object.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::object,
    &minty::core::db::object::id,
    &minty::core::db::object::preview_id,
    &minty::core::db::object::src
);
