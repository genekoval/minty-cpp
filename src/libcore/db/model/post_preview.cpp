#include <internal/core/db/model/post_preview.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::post_preview,
    &minty::core::db::post_preview::id,
    &minty::core::db::post_preview::title,
    &minty::core::db::post_preview::preview,
    &minty::core::db::post_preview::comment_count,
    &minty::core::db::post_preview::object_count,
    &minty::core::db::post_preview::date_created
);
