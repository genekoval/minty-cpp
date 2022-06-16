#include "parser.h"

using minty::cli::output::format;
using minty::cli::output::format_string;

namespace commline {
    auto parser<format>::parse(std::string_view argument) -> format {
        const auto it = std::find(
            format_string.begin(),
            format_string.end(),
            argument
        );

        if (it == format_string.end()) {
            throw commline::cli_error("unknown format type: {}", argument);
        }

        const auto result = std::distance(format_string.begin(), it);
        return static_cast<format>(result);
    }
}
