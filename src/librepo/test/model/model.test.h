#pragma once

#include <entix/entix>

namespace minty::test {
    struct sequence_object {
        std::string id;
        unsigned int sequence;
    };
}

namespace entix {
    ENTITY(minty::test::sequence_object);
}
