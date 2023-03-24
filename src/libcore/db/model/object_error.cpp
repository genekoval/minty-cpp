#include <internal/core/db/model/object_error.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::object_error,
    &minty::object_error::id,
    &minty::object_error::message
);
