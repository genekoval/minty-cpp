#pragma once

#include <commline/commline>

namespace minty::cli::opts {
    auto add_tags() -> commline::list<std::string>;

    auto aliases() -> commline::list<std::string_view>;

    auto description() -> commline::option<std::string>;

    auto from() -> commline::option<unsigned int>;

    auto links() -> commline::list<std::string_view>;

    auto path() -> commline::option<std::optional<std::string_view>>;

    auto size() -> commline::option<unsigned int>;

    auto tags() -> commline::list<std::string>;

    auto title() -> commline::option<std::string>;
}
