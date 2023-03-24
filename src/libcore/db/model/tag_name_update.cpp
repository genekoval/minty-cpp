#include <internal/core/db/model/tag_name_update.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::tag_name_update,
    &minty::core::db::tag_name_update::names,
    &minty::core::db::tag_name_update::old_name
);
