#pragma once

#include <commline/commline>

namespace minty::subcommands::comment {
    auto add() -> std::unique_ptr<commline::command_node>;

    auto edit() -> std::unique_ptr<commline::command_node>;
}
