#pragma once

#include <commline/commline>

namespace minty::subcommands::post_tags {
    auto add() -> std::unique_ptr<commline::command_node>;

    auto rm() -> std::unique_ptr<commline::command_node>;
}
