#include "opts.h"

using namespace commline;

namespace minty::cli::opts {
    auto from() -> commline::option<int> {
        return option<int>(
            {"f", "from"},
            "Result offset",
            "number",
            0
        );
    }

    auto path() -> commline::option<std::optional<std::string_view>> {
        return option<std::optional<std::string_view>>(
            {"S", "select"},
            "Select YAML output",
            "path"
        );
    }

    auto size() -> commline::option<int> {
        return option<int>(
            {"n", "size"},
            "Result size",
            "number",
            10
        );
    }

    auto tags() -> commline::list<std::string> {
        return list<std::string>(
            {"t", "tag"},
            "Post tags",
            "id"
        );
    }
}
