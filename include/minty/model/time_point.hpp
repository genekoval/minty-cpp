#pragma once

#include <chrono>

namespace minty {
    using time_point = std::chrono::time_point<
        std::chrono::system_clock,
        std::chrono::milliseconds
    >;
}
