#include "../client.h"
#include "../output.h"

#include "commands.h"

#include <minty/minty>

#include <iostream>

static auto $creator(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.empty()) {
        return;
    }

    auto api = minty::cli::client();
    const auto creator = api.get_creator(argv[0]);

    auto out = YAML::Emitter();
    out << creator;
    std::cout << out.c_str() << std::endl;
}

static auto $add(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.empty()) {
        return;
    }

    auto api = minty::cli::client();
    std::cout << api.add_creator(argv[0]) << std::endl;
}

namespace minty::commands {
    using namespace commline;

    auto creator_add() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a creator",
            $add
        );
    }

    auto creator() -> std::unique_ptr<command_node> {
        auto cmd = command(
            "creator",
            "Manage creators",
            $creator
        );

        cmd->subcommand(creator_add());

        return cmd;
    }
}
