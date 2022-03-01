#include "../client.h"
#include "../output.h"
#include "../options/opts.h"
#include "../parser/parser.h"

#include "commands.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

static auto $post(
    const commline::app& app,
    std::string_view id
) -> void {
    auto api = minty::cli::client();
    const auto post = api.get_post(id);

    auto out = YAML::Emitter();
    out << post;

    const auto comments = api.get_comments(id);

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
    std::string_view title,
    std::string_view description,
    const std::vector<std::string>& tags,
    const std::vector<std::string_view>& objects
) -> void {
    auto api = minty::cli::client();

    auto parts = minty::core::post_parts {
        .title = title.empty() ?
            std::optional<std::string>() :
            std::optional<std::string>(title),
        .description = description.empty() ?
            std::optional<std::string>() :
            std::optional<std::string>(description),
        .tags = tags
    };

    for (const auto& object : objects) {
        const auto path = fs::canonical(object).string();
        parts.objects.emplace_back(api.add_object_local(path));
    }

    const auto id = api.add_post(parts);

    std::cout << id << std::endl;
}

static auto $find(
    const commline::app& app,
    int from,
    int size,
    const std::vector<std::string>& tags,
    const minty::core::post_query::sort_type sort,
    std::optional<std::string_view> path,
    std::optional<std::string> text
) -> void {
    auto api = minty::cli::client();

    const auto query = minty::core::post_query {
        .from = static_cast<unsigned int>(from),
        .size = static_cast<unsigned int>(size),
        .text = text,
        .tags = tags,
        .sort = sort
    };

    const auto result = api.get_posts(query);
    minty::cli::print(result, path);
}

static auto $rm(
    const commline::app& app,
    bool force,
    std::string_view id
) -> void {
    auto api = minty::cli::client();

    if (!force) {
        std::cout
            << "Remove post with ID: (" << id << ")? [yes/no] ";

        auto response = std::array<char, 4>();
        std::cin.getline(response.data(), response.size());
        const auto res = std::string(response.data(), response.size());

        if (res != "yes") return;
    }

    api.delete_post(id);
}

namespace minty::commands {
    using namespace commline;

    auto post_add() -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Add a post",
            options(
                option<std::string_view>(
                    {"T", "title"},
                    "An optional post title",
                    "text"
                ),
                option<std::string_view>(
                    {"d", "description"},
                    "An optional post description",
                    "text"
                ),
                cli::opts::tags()
            ),
            arguments(
                variadic<std::string_view>("objects")
            ),
            $add
        );
    }

    auto post_find() -> std::unique_ptr<command_node> {
        return command(
            "find",
            "Search for posts",
            options(
                cli::opts::from(),
                cli::opts::size(),
                cli::opts::tags(),
                option<core::post_query::sort_type>(
                    {"s", "sort-by"},
                    "Result sort",
                    "",
                    {
                        .order = core::sort_order::descending,
                        .value = core::post_sort_value::date_created
                    }
                ),
                cli::opts::path()
            ),
            arguments(
                optional<std::string>("text")
            ),
            $find
        );
    }

    auto post_rm() -> std::unique_ptr<command_node> {
        return command(
            "rm",
            "Remove a post",
            options(
                flag(
                    {"f", "force"},
                    "Remove the post without prompting for confirmation"
                )
            ),
            arguments(
                required<std::string_view>("id")
            ),
            $rm
        );
    }

    auto post() -> std::unique_ptr<command_node> {
        auto cmd = command(
            "post",
            "Get post information",
            options(),
            arguments(
                required<std::string_view>("id")
            ),
            $post
        );

        cmd->subcommand(post_add());
        cmd->subcommand(post_find());
        cmd->subcommand(post_rm());

        return cmd;
    }
}
