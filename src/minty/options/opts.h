#pragma once

#include "../output/output.h"

#include <commline/commline>
#include <uuid++/uuid++>

namespace minty::cli::opts {
    auto add_tags() -> commline::list<UUID::uuid>;

    auto aliases() -> commline::list<std::string_view>;

    auto description() -> commline::option<std::string>;

    auto from() -> commline::option<unsigned int>;

    auto links() -> commline::list<std::string_view>;

    auto output() ->
        commline::option<std::optional<minty::cli::output::format>>;

    auto path() -> commline::option<std::optional<std::string_view>>;

    auto quiet() -> commline::flag;

    auto size() -> commline::option<unsigned int>;

    auto tags() -> commline::list<UUID::uuid>;

    auto title() -> commline::option<std::string>;
}
