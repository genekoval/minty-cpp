#include <internal/core/db/model/site.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::site,
    &minty::core::db::site::id,
    &minty::core::db::site::scheme,
    &minty::core::db::site::host,
    &minty::core::db::site::icon
);
