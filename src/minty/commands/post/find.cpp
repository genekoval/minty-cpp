#include "commands.h"

#include "../../client.h"
#include "../../options/opts.h"
#include "../../output.h"
#include "../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto find(
            const app& app,
            unsigned int from,
            unsigned int size,
            const std::vector<std::string>& tags,
            const minty::core::post_query::sort_type sort,
            std::optional<std::string_view> path,
            std::optional<std::string> text
        ) -> void {
            auto api = minty::cli::client();

            const auto query = minty::core::post_query {
                .from = from,
                .size = size,
                .text = text,
                .tags = tags,
                .sort = sort
            };

            const auto result = api.get_posts(query);
            minty::cli::print(result, path);
        }
    }
}

namespace minty::subcommands::post {
    auto find() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
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
            internal::find
        );
    }
}
