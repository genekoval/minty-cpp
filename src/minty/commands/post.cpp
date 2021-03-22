#include "../client.h"
#include "../output.h"

#include "commands.h"

#include <minty/minty>

#include <filesystem>
#include <iostream>

static auto $post(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.empty()) {
        throw std::runtime_error("no post id given");
    }

    const auto& post_id = argv.front();
    auto api = minty::cli::client();
    const auto post = api.get_post(post_id);

    auto out = YAML::Emitter();
    out << post;
    std::cout << out.c_str() << std::endl;
}

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
        auto path = cwd / file;

        if (std::filesystem::is_regular_file(path)) {
            files.push_back(path);
        }
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
