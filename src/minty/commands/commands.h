#pragma once

#include <commline/commline>

namespace minty::commands {
    auto post() -> std::unique_ptr<commline::command_node>;
    auto tag() -> std::unique_ptr<commline::command_node>;
}
