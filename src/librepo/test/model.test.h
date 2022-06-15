#pragma once

#include <entix/entix>

namespace minty::test {
    struct sequence_object {
        UUID::uuid id;
        unsigned int sequence;
    };
}

namespace entix {
    ENTITY(
        minty::test::sequence_object,
        &minty::test::sequence_object::id,
        &minty::test::sequence_object::sequence
    );
}
