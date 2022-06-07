#pragma once

#include <commline/commline>

namespace minty::cli::sub::regen {
    auto all(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;
}
