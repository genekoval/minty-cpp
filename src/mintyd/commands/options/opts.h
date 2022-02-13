#pragma once

#include <commline/commline>

namespace minty::cli::opts {
    auto config(
        std::string_view confpath
    ) -> commline::option<std::string_view>;
}
