#include "opts.h"

using namespace commline;

namespace minty::cli::opts {
    auto config(
        std::string_view confpath
    ) -> commline::option<std::string_view> {
        return option<std::string_view>(
            {"c", "config"},
            "Path to configuration file",
            "path",
            std::move(confpath)
        );
    }
}
