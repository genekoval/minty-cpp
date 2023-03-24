#include "model.test.hpp"

PGCPP_COMPOSITE_DEFINE(
    minty::test::sequence_object,
    &minty::test::sequence_object::id,
    &minty::test::sequence_object::sequence
);
