#include "../client.h"
#include "../output.h"

#include "commands.h"

#include <iostream>

static auto print_tag(
    minty::api& api,
    std::string_view tag_id,
    std::optional<std::string_view> path
) -> void {
    minty::cli::print(api.get_tag(tag_id), path);
}

static auto print_tag(minty::api& api, std::string_view tag_id) -> void {
    print_tag(api, tag_id, {});
}

static auto $tag(
    const commline::app& app,
    const commline::argv& argv,
    std::optional<std::string_view> path,
    std::optional<std::string_view> description,
    std::optional<std::string_view> link
) -> void {
    if (argv.empty()) throw std::runtime_error(
        "missing tag id"
    );

    const auto& id = argv.front();
    auto api = minty::cli::client();

    if (description) api.set_tag_description(id, *description);

    if (link) api.add_tag_source(id, *link);

    print_tag(api, id, path);
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

static auto $alias(
    const commline::app& app,
    const commline::argv& argv
) -> void {}

static auto $alias_add(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.size() < 2) return;

    const auto& id = argv[0];
    const auto& alias = argv[1];

    auto api = minty::cli::client();

    api.add_tag_alias(id, alias);
    print_tag(api, id);
}

static auto $alias_rm(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.size() < 2) return;

    const auto& id = argv[0];
    const auto& alias = argv[1];

    auto api = minty::cli::client();

    api.delete_tag_alias(id, alias);
    print_tag(api, id);
}

static auto $find(
    const commline::app& app,
    const commline::argv& argv,
    int from,
    std::optional<std::string_view> path,
    int size
) -> void {
    if (argv.empty()) return;

    const auto& search_term = argv.front();

    const auto query = minty::core::tag_query {
        .from = static_cast<unsigned int>(from),
        .size = static_cast<unsigned int>(size),
        .name = std::string(search_term)
    };

    auto api = minty::cli::client();
    minty::cli::print(api.get_tags(query), path);
}

static auto $rename(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.size() < 2) {
        throw std::runtime_error("tag id and new name required");
    }

    const auto& id = argv[0];
    const auto& name = argv[1];

    auto api = minty::cli::client();

    api.set_tag_name(id, name);
    print_tag(api, id);
}

static auto $rm(
    const commline::app& app,
    const commline::argv& argv,
    bool force
) -> void {
    auto api = minty::cli::client();

    if (argv.empty()) {
        throw std::runtime_error("no tag id given");
    }

    const auto& tag = argv.front();

    if (!force) {
        std::cout
            << "Remove tag with ID: (" << tag << ")? [yes/no] ";

        auto response = std::array<char, 4>();
        std::cin.getline(response.data(), response.size());
        const auto res = std::string(response.data(), response.size());

        if (res != "yes") return;
    }

    api.delete_tag(tag);
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

    auto tag_alias_add() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a tag alias",
            $alias_add
        );
    }

    auto tag_alias_rm() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a tag alias",
            $alias_rm
        );
    }

    auto tag_alias() -> std::unique_ptr<command_node> {
        auto cmd = command(
            "alias",
            "Manage tag aliases",
            $alias
        );

        cmd->subcommand(tag_alias_add());
        cmd->subcommand(tag_alias_rm());

        return cmd;
    }

    auto tag_find() -> std::unique_ptr<command_node> {
        return command(
            "find",
            "Find tags",
            options(
                option<int>(
                    {"from", "f"},
                    "Result offset",
                    "number",
                    0
                ),
                option<std::optional<std::string_view>>(
                    {"select", "S"},
                    "Select YAML output",
                    "path"
                ),
                option<int>(
                    {"size", "s"},
                    "Result size",
                    "number",
                    10
                )
            ),
            $find
        );
    }

    auto tag_rename() -> std::unique_ptr<command_node> {
        return command(
            "rename",
            "Change a tag's name",
            $rename
        );
    }

    auto tag_rm() -> std::unique_ptr<command_node> {
        return command(
            "rm",
            "Remove a tag",
            options(
                flag(
                    {"force", "f"},
                    "Remove the tag without prompting for confirmation."
                )
            ),
            $rm
        );
    }

    auto tag() -> std::unique_ptr<command_node> {
        auto cmd = command(
            "tag",
            "View or edit a tag",
            options(
                option<std::optional<std::string_view>>(
                    {"select", "S"},
                    "Select YAML output",
                    "path"
                ),
                option<std::optional<std::string_view>>(
                    {"description", "d"},
                    "Set the tag's description",
                    "new description"
                ),
                option<std::optional<std::string_view>>(
                    {"link", "l"},
                    "Add a link to the tag",
                    "url"
                )
            ),
            $tag
        );

        cmd->subcommand(tag_add());
        cmd->subcommand(tag_alias());
        cmd->subcommand(tag_find());
        cmd->subcommand(tag_rename());
        cmd->subcommand(tag_rm());

        return cmd;
    }
}
