#include <internal/core/db/model/post_update.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::post_update,
    &minty::core::db::post_update::id,
    &minty::core::db::post_update::new_data,
    &minty::core::db::post_update::date_modified
);
