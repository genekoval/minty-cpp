#include "../client.h"
#include "../output.h"

#include "commands.h"

#include <minty/minty>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

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

    const auto comments = api.get_comments(post_id);

    if (!comments.empty()) {
        out << YAML::BeginMap
            << YAML::Key << "comments" << YAML::BeginSeq;

        for (const auto& comment : comments) {
            out << comment;
        }

        out << YAML::EndSeq;
    }

    std::cout << out.c_str() << std::endl;
}

static auto $add(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view title,
    std::string_view description,
    std::string_view tag
) -> void {
    auto api = minty::cli::client();

    auto parts = minty::core::post_parts {
        .title = title.empty() ?
            std::optional<std::string>() :
            std::optional<std::string>(title),
        .description = description.empty() ?
            std::optional<std::string>() :
            std::optional<std::string>(description)
    };

    for (const auto& arg : argv) {
        const auto path = fs::canonical(arg).string();
        parts.objects.emplace_back(api.add_object_local(path));
    }

    parts.tags.emplace_back(tag);

    const auto id = api.add_post(parts);

    std::cout << id << std::endl;
}

static auto $rm(
    const commline::app& app,
    const commline::argv& argv,
    bool force
) -> void {
    auto api = minty::cli::client();

    if (argv.empty()) {
        throw std::runtime_error("no post id given");
    }

    const auto& post = argv.front();

    if (!force) {
        std::cout
            << "Remove post with ID: (" << post << ")? [yes/no] ";

        auto response = std::array<char, 4>();
        std::cin.getline(response.data(), response.size());
        const auto res = std::string(response.data(), response.size());

        if (res != "yes") return;
    }

    api.delete_post(post);
}

namespace minty::commands {
    using namespace commline;

    auto post_add() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a post",
            options(
                option<std::string_view>(
                    {"title", "T"},
                    "Post title",
                    "text"
                ),
                option<std::string_view>(
                    {"description", "d"},
                    "Post description",
                    "text"
                ),
                option<std::string_view>(
                    {"tag", "t"},
                    "Post tag",
                    "id"
                )
            ),
            $add
        );
    }

    auto post_rm() -> std::unique_ptr<command_node> {
        return command(
            "rm",
            "Remove a post",
            options(
                flag(
                    {"force", "f"},
                    "Remove the post without prompting for confirmation."
                )
            ),
            $rm
        );
    }

    auto post() -> std::unique_ptr<command_node> {
        auto cmd = command(
            "post",
            "Manage posts",
            $post
        );

        cmd->subcommand(post_add());
        cmd->subcommand(post_rm());

        return cmd;
    }
}
