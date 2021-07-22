#pragma once

#include <commline/commline>

namespace minty::cli {
    auto init(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto migrate(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;
}
