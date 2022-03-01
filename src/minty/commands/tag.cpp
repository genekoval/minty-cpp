#include "../client.h"
#include "../output.h"
#include "../options/opts.h"

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
    std::optional<std::string_view> path,
    std::optional<std::string_view> description,
    std::optional<std::string_view> link,
    std::string_view id
) -> void {
    auto api = minty::cli::client();

    if (description) api.set_tag_description(id, *description);
    if (link) api.add_tag_source(id, *link);

    print_tag(api, id, path);
}

static auto $add(
    const commline::app& app,
    std::string_view name
) -> void {
    auto api = minty::cli::client();
    std::cout << api.add_tag(name) << std::endl;
}

static auto $alias(
    const commline::app& app
) -> void {}

static auto $alias_add(
    const commline::app& app,
    std::string_view id,
    std::string_view alias
) -> void {
    auto api = minty::cli::client();

    api.add_tag_alias(id, alias);
    print_tag(api, id);
}

static auto $alias_rm(
    const commline::app& app,
    std::string_view id,
    std::string_view alias
) -> void {
    auto api = minty::cli::client();

    api.delete_tag_alias(id, alias);
    print_tag(api, id);
}

static auto $find(
    const commline::app& app,
    int from,
    int size,
    std::optional<std::string_view> path,
    std::string name
) -> void {
    const auto query = minty::core::tag_query {
        .from = static_cast<unsigned int>(from),
        .size = static_cast<unsigned int>(size),
        .name = name
    };

    auto api = minty::cli::client();
    minty::cli::print(api.get_tags(query), path);
}

static auto $rename(
    const commline::app& app,
    std::string_view id,
    std::string_view name
) -> void {
    auto api = minty::cli::client();

    api.set_tag_name(id, name);
    print_tag(api, id);
}

static auto $rm(
    const commline::app& app,
    bool force,
    std::string_view id
) -> void {
    auto api = minty::cli::client();

    if (!force) {
        std::cout
            << "Remove tag with ID: (" << id << ")? [yes/no] ";

        auto response = std::array<char, 4>();
        std::cin.getline(response.data(), response.size());
        const auto res = std::string(response.data(), response.size());

        if (res != "yes") return;
    }

    api.delete_tag(id);
}

namespace minty::commands {
    using namespace commline;

    auto tag_add() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a tag",
            options(),
            arguments(
                required<std::string_view>("name")
            ),
            $add
        );
    }

    auto tag_alias_add() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a tag alias",
            options(),
            arguments(
                required<std::string_view>("id"),
                required<std::string_view>("alias")
            ),
            $alias_add
        );
    }

    auto tag_alias_rm() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a tag alias",
            options(),
            arguments(
                required<std::string_view>("id"),
                required<std::string_view>("alias")
            ),
            $alias_rm
        );
    }

    auto tag_alias() -> std::unique_ptr<command_node> {
        auto cmd = command(
            "alias",
            "Manage tag aliases",
            options(),
            arguments(),
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
                cli::opts::from(),
                cli::opts::size(),
                cli::opts::path()
            ),
            arguments(
                required<std::string>("name")
            ),
            $find
        );
    }

    auto tag_rename() -> std::unique_ptr<command_node> {
        return command(
            "rename",
            "Change a tag's name",
            options(),
            arguments(
                required<std::string_view>("id"),
                required<std::string_view>("name")
            ),
            $rename
        );
    }

    auto tag_rm() -> std::unique_ptr<command_node> {
        return command(
            "rm",
            "Remove a tag",
            options(
                flag(
                    {"f", "force"},
                    "Remove the tag without prompting for confirmation."
                )
            ),
            arguments(
                required<std::string_view>("id")
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
                    {"d", "description"},
                    "Set the tag's description",
                    "text"
                ),
                option<std::optional<std::string_view>>(
                    {"l", "link"},
                    "Add a link to the tag",
                    "url"
                )
            ),
            arguments(
                required<std::string_view>("id")
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
