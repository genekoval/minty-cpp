#include <internal/core/db/model/tag.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::tag,
    &minty::core::db::tag::id,
    &minty::core::db::tag::name,
    &minty::core::db::tag::aliases,
    &minty::core::db::tag::description,
    &minty::core::db::tag::avatar,
    &minty::core::db::tag::banner,
    &minty::core::db::tag::post_count,
    &minty::core::db::tag::date_created
);
