#include <internal/core/db/model/tag_name.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::tag_name,
    &minty::tag_name::name,
    &minty::tag_name::aliases
);
