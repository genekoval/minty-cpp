#pragma once

#include <commline/commline>

namespace minty::cli {
    auto db(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto dump(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto errors(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto init(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto migrate(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto prune(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto regen(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto reindex(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto restore(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;
}
