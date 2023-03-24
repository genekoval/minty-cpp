#pragma once

#include <pg++/pg++>

namespace minty::test {
    struct sequence_object {
        UUID::uuid id;
        std::int16_t sequence;
    };
}

PGCPP_COMPOSITE_DECL(minty::test::sequence_object, "sequence_object");
