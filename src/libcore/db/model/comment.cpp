#include <internal/core/db/model/comment.hpp>

PGCPP_COMPOSITE_DEFINE(
    minty::comment,
    &minty::comment::id,
    &minty::comment::post_id,
    &minty::comment::parent_id,
    &minty::comment::indent,
    &minty::comment::content,
    &minty::comment::date_created
);
