#include "commands.h"

#include "../../client.h"
#include "../../options/opts.h"
#include "../../output/output.h"
#include "../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto ln(
            const app& app,
            bool json,
            bool quiet,
            const UUID::uuid& id,
            std::string_view url
        ) -> void {
            auto api = minty::cli::client();

            auto tag = api.get_tag(id);

            const auto end = tag.sources.end();
            const auto result = std::ranges::find_if(
                tag.sources.begin(),
                end,
                [url](const auto& source) { return source.url == url; }
            );

            if (result != end) {
                api.delete_tag_source(id, result->id);
                tag.sources.erase(result);
            }
            else {
                const auto source = api.add_tag_source(id, url);
                tag.sources.push_back(source);
            }

            minty::cli::output::entity(tag, json, !quiet);
        }
    }
}

namespace minty::subcommands::tag {
    auto ln() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Add or remove a tag's link",
            options(
                cli::opts::json(),
                cli::opts::quiet()
            ),
            arguments(
                required<UUID::uuid>("id"),
                required<std::string_view>("url")
            ),
            internal::ln
        );
    }
}
