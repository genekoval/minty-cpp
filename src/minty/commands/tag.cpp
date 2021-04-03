#include "../client.h"
#include "../output.h"

#include "commands.h"

#include <minty/minty>

#include <iostream>

static auto $tag(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.empty()) {
        return;
    }

    auto api = minty::cli::client();
    const auto tag = api.get_tag(argv[0]);

    auto out = YAML::Emitter();
    out << tag;
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
    std::cout << api.add_tag(argv[0]) << std::endl;
}

static auto $find(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.empty()) return;

    auto api = minty::cli::client();
    const auto tags = api.get_tags_by_name(argv[0]);

    auto out = YAML::Emitter();
    out << YAML::BeginSeq;

    for (const auto& tag : tags) {
        out << tag;
    }

    out << YAML::EndSeq;

    std::cout << out.c_str() << std::endl;
}

namespace minty::commands {
    using namespace commline;

    auto tag_add() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a tag",
            $add
        );
    }

    auto tag_find() -> std::unique_ptr<command_node> {
        return command(
            "find",
            "Find tags",
            $find
        );
    }

    auto tag() -> std::unique_ptr<command_node> {
        auto cmd = command(
            "tag",
            "Manage tags",
            $tag
        );

        cmd->subcommand(tag_add());
        cmd->subcommand(tag_find());

        return cmd;
    }
}
