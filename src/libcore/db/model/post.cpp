#include <internal/core/db/model/post.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::post,
    &minty::core::db::post::id,
    &minty::core::db::post::title,
    &minty::core::db::post::description,
    &minty::core::db::post::date_created,
    &minty::core::db::post::date_modified
);
