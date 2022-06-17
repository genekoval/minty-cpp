#pragma once

#include "../output/output.h"

#include <commline/commline>
#include <uuid++/uuid++>

namespace minty::cli::opts {
    auto add_tags() -> commline::list<UUID::uuid>;

    auto description() -> commline::option<std::string>;

    /**
     * Result offset.
     */
    auto from() -> commline::option<unsigned int>;

    /**
     * Print result as JSON to STDOUT.
     */
    auto json() -> commline::flag;

    auto output() ->
        commline::option<std::optional<minty::cli::output::format>>;

    auto path() -> commline::option<std::optional<std::string_view>>;

    /**
     * Suppress human-readable output.
     */
    auto quiet() -> commline::flag;

    /**
     * Result size
     */
    auto size() -> commline::option<unsigned int>;

    auto tags() -> commline::list<UUID::uuid>;

    auto title() -> commline::option<std::string>;
}
