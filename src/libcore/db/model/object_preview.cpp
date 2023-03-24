#include <internal/core/db/model/object_preview.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::object_preview,
    &minty::core::db::object_preview::id,
    &minty::core::db::object_preview::preview_id
);
