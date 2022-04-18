#include "commands.h"

#include "../../client.h"
#include "../../options/opts.h"
#include "../../output.h"
#include "../../parser/parser.h"

using namespace commline;

namespace {
    auto $tag(
        const app& app,
        std::optional<std::string_view> name,
        const std::vector<std::string_view>& new_aliases,
        const std::vector<std::string_view>& aliases_to_remove,
        std::optional<std::string> description,
        const std::vector<std::string_view>& new_links,
        const std::vector<std::string_view>& links_to_remove,
        std::optional<std::string_view> path,
        const UUID::uuid& id
    ) -> void {
        auto api = minty::cli::client();

        if (name) api.set_tag_name(id, *name);
        if (description) api.set_tag_description(id, *description);

        for (const auto& alias : new_aliases) {
            api.add_tag_alias(id, alias);
        }

        for (const auto& alias : aliases_to_remove) {
            api.delete_tag_alias(id, alias);
        }

        for (const auto& link : new_links) {
            api.add_tag_source(id, link);
        }

        for (const auto& link : links_to_remove) {
            api.delete_tag_source(id, link);
        }

        minty::cli::print_tag(api, id, path);
    }
}

namespace minty::commands {
    auto tag() -> std::unique_ptr<command_node> {
        auto cmd = command(
            __FUNCTION__,
            "View or edit a tag",
            options(
                option<std::optional<std::string_view>>(
                    {"n", "name"},
                    "Set the tag's primary name",
                    "text"
                ),
                cli::opts::aliases(),
                list<std::string_view>(
                    {"alias-rm"},
                    "Remove an alias",
                    "name"
                ),
                cli::opts::description(),
                cli::opts::links(),
                list<std::string_view>(
                    {"link-rm"},
                    "Remove a link",
                    "url"
                ),
                cli::opts::path()
            ),
            arguments(
                required<UUID::uuid>("id")
            ),
            $tag
        );

        cmd->subcommand(subcommands::tag::add());
        cmd->subcommand(subcommands::tag::find());
        cmd->subcommand(subcommands::tag::rm());

        return cmd;
    }
}
