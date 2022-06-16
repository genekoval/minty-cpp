#include "commands.h"

#include "../../client.h"
#include "../../options/opts.h"
#include "../../output/output.h"
#include "../../parser/parser.h"

using namespace commline;

namespace output = minty::cli::output;

namespace {
    namespace internal {
        auto find(
            const commline::app& app,
            unsigned int from,
            unsigned int size,
            const std::optional<output::format>& format,
            bool quiet,
            const std::string& name
        ) -> void {
            const auto query = minty::core::tag_query {
                .from = from,
                .size = size,
                .name = name
            };

            auto api = minty::cli::client();
            const auto result = api.get_tags(query);

            output::result(result, format, quiet);
        }
    }
}

namespace minty::subcommands::tag {
    auto find() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Find tags",
            options(
                cli::opts::from(),
                cli::opts::size(),
                cli::opts::output(),
                cli::opts::quiet()
            ),
            arguments(
                required<std::string>("name")
            ),
            internal::find
        );
    }
}
