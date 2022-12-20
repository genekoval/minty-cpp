#pragma once

#include <uuid++/uuid++>

namespace minty {
    struct object_error {
        UUID::uuid id;
        std::string message;
    };
}
