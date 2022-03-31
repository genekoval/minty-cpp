#include "parser.h"

using namespace std::literals;

using minty::core::sort_order;
using sort_value = minty::core::post_sort_value;

namespace {
    constexpr auto ascending = "ascending"sv;
    constexpr auto descending = "descending"sv;

    constexpr auto delimiter = ".";

    constexpr auto value_types = std::array {
        "created",
        "modified",
        "relevance",
        "title"
    };

    auto default_order(sort_value value) -> sort_order {
        if (
            value == sort_value::date_created ||
            value == sort_value::date_modified
        ) return sort_order::descending;

        return sort_order::ascending;
    }

    auto get_order(std::string_view argument) -> sort_order {
        if (ascending.starts_with(argument)) {
            return sort_order::ascending;
        }

        if (descending.starts_with(argument)) {
            return sort_order::descending;
        }

        throw commline::cli_error(
            "unknown sort order: " +
            std::string(argument)
        );
    }

    auto get_value(
        std::string_view argument
    ) -> sort_value {
        const auto it = std::find(
            value_types.begin(),
            value_types.end(),
            argument
        );

        if (it == value_types.end()) {
            throw commline::cli_error(
                "unknown sort type: " +
                std::string(argument)
            );
        }

        const auto result = std::distance(value_types.begin(), it);
        return static_cast<sort_value>(result);
    }
}

namespace commline {
    auto parser<minty::core::post_query::sort_type>::parse(
        std::string_view argument
    ) -> minty::core::post_query::sort_type {
        const auto delim = argument.find(delimiter);

        const auto value = get_value(argument.substr(0, delim));
        const auto order = delim == std::string_view::npos ?
            default_order(value) : get_order(argument.substr(0, delim));

        return {
            .order = order,
            .value = value
        };
    }
}
