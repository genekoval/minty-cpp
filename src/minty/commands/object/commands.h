#pragma once

#include <commline/commline>

namespace minty::subcommands::object {
    auto get() -> std::unique_ptr<commline::command_node>;
}
