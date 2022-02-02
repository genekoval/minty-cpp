#include "options.h"

using namespace commline;

namespace minty::cli::options {
    auto config(
        std::string_view confpath
    ) -> commline::option<std::string_view> {
        return option<std::string_view>(
            {"config", "c"},
            "Path to configuration file",
            "path",
            std::move(confpath)
        );
    }
}
