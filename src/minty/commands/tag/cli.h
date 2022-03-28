#pragma once

#include <minty/minty>

namespace minty::cli {
    auto print_tag(
        minty::api& api,
        std::string_view id,
        std::optional<std::string_view> path
    ) -> void;
}
