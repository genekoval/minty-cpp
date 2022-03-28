#pragma once

#include <commline/commline>

namespace minty::cli::opts {
    auto aliases() -> commline::list<std::string_view>;

    auto description() -> commline::option<std::optional<std::string_view>>;

    auto from() -> commline::option<int>;

    auto links() -> commline::list<std::string_view>;

    auto path() -> commline::option<std::optional<std::string_view>>;

    auto size() -> commline::option<int>;

    auto tags() -> commline::list<std::string>;
}
