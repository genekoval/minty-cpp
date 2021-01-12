#include <algorithm>
#include <iostream>
#include <timber/timber>

namespace timber {
    auto handle_log(const log& l) noexcept -> void {
        if (l.log_level != timber::level::info) {
            std::cerr << l.log_level << ": ";
        }

        std::cerr << l.stream.str() << std::endl;
    }
}
