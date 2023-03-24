#include <internal/core/db/model/tag_preview.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::tag_preview,
    &minty::tag_preview::id,
    &minty::tag_preview::name,
    &minty::tag_preview::avatar
);
