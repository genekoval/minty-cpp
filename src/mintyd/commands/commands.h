#pragma once

#include <commline/commline>

namespace minty::cli {
    auto stop(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;
}
