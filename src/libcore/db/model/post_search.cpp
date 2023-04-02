#include <internal/core/db/model/post_search.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::post_search,
    &minty::core::db::post_search::id,
    &minty::core::db::post_search::title,
    &minty::core::db::post_search::description,
    &minty::core::db::post_search::visibility,
    &minty::core::db::post_search::created,
    &minty::core::db::post_search::modified,
    &minty::core::db::post_search::tags
);
