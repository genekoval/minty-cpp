#include "../../repo/repo.hpp"
#include "../commands.h"
#include "../options/opts.h"
#include "commands.h"

#include <uuid++/commline>

using namespace commline;

namespace {
    namespace internal {
        auto regen(
            const commline::app& app,
            std::string_view confpath,
            const UUID::uuid& id
        ) -> void {
            const auto settings = minty::conf::initialize(confpath);

            minty::cli::repo(
                settings,
                [&id](minty::core::repo& repo) -> ext::task<> {
                    const auto preview = co_await repo.regenerate_preview(id);
                    if (preview) fmt::print("{}\n", *preview);
                }
            );
        }
    }
}

namespace minty::cli {
    auto regen(std::string_view confpath) -> std::unique_ptr<command_node> {
        auto cmd = command(
            __FUNCTION__,
            "Regenerate object previews",
            options(opts::config(confpath)),
            arguments(required<UUID::uuid>("id")),
            internal::regen
        );

        cmd->subcommand(sub::regen::all(confpath));

        return cmd;
    }
}
