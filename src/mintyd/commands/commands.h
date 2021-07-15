#pragma once

#include <commline/commline>

namespace minty::cli {
    auto reindex(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto stop(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;
}
