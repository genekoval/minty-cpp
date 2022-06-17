#include "commands.h"

#include "../../client.h"
#include "../../options/opts.h"
#include "../../output/output.h"
#include "../../parser/parser.h"

using namespace commline;

namespace {
    namespace internal {
        auto rename(
            const app& app,
            bool quiet,
            const UUID::uuid& id,
            std::string_view name
        ) -> void {
            auto api = minty::cli::client();

            const auto names = api.set_tag_name(id, name);

            if (quiet) return;

            fmt::print("{}\n", names.name);

            if (!names.aliases.empty()) {
                fmt::print("\n");

                for (const auto& alias : names.aliases) {
                    fmt::print("{}\n", alias);
                }
            }
        }
    }
}

namespace minty::subcommands::tag {
    auto rename() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Set a tag's primary name",
            options(
                cli::opts::quiet()
            ),
            arguments(
                required<UUID::uuid>("id"),
                required<std::string_view>("name")
            ),
            internal::rename
        );
    }
}
