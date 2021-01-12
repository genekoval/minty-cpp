#pragma once

#include <commline/commline>

namespace minty::commands {
    auto creator() -> std::unique_ptr<commline::command_node>;
}
