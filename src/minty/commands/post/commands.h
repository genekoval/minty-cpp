#pragma once

#include <commline/commline>

namespace minty::subcommands::post {
    auto add() -> std::unique_ptr<commline::command_node>;

    auto description() -> std::unique_ptr<commline::command_node>;

    auto find() -> std::unique_ptr<commline::command_node>;

    auto objects() -> std::unique_ptr<commline::command_node>;

    auto related() -> std::unique_ptr<commline::command_node>;

    auto rm() -> std::unique_ptr<commline::command_node>;

    auto tags() -> std::unique_ptr<commline::command_node>;

    auto title() -> std::unique_ptr<commline::command_node>;
}
