#pragma once

#include <commline/commline>

namespace minty::cli::opts {
    auto from() -> commline::option<int>;

    auto path() -> commline::option<std::optional<std::string_view>>;

    auto size() -> commline::option<int>;

    auto tags() -> commline::list<std::string>;
}
