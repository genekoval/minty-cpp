#include <internal/core/db/model/tag_search.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::tag_search,
    &minty::core::db::tag_search::id,
    &minty::core::db::tag_search::names
);
