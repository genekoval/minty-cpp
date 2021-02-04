#include "../client.h"

#include "commands.h"

#include <minty/minty>

#include <filesystem>
#include <iostream>

static auto $post(
    const commline::app& app,
    const commline::argv& argv
) -> void {}

static auto $add(
    const commline::app& app,
    const commline::argv& argv,
    std::optional<std::string_view> description,
    std::optional<std::string_view> creator
) -> void {
    auto api = minty::cli::client();

    auto files = std::vector<std::string>();

    for (const auto file : argv) {
        auto cwd = std::filesystem::current_path();
        files.push_back(cwd / file);
    }

    auto id = api.add_post(
        description,
        files,
        creator,
        {}
    );

    std::cout << id << std::endl;
}

namespace minty::commands {
    using namespace commline;

    auto post_add() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a post",
            options(
                option<std::string_view>(
                    {"description", "d"},
                    "Post description",
                    "text"
                ),
                option<std::string_view>(
                    {"creator", "c"},
                    "Creator ID",
                    "id"
                )
            ),
            $add
        );
    }

    auto post() -> std::unique_ptr<command_node> {
        auto cmd = command(
            "post",
            "Manage posts",
            $post
        );

        cmd->subcommand(post_add());

        return cmd;
    }
}
