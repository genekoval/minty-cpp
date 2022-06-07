#include "commands.h"
#include "../../api/api.h"
#include "../commands.h"
#include "../options/opts.h"

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
            auto container = minty::cli::api_container(settings);
            auto& api = container.api();

            const auto preview = api.regenerate_preview(id);
            if (preview) std::cout << *preview;
        }
    }
}

namespace minty::cli {
    auto regen(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        auto cmd = command(
            __FUNCTION__,
            "Regenerate object previews",
            options(
                opts::config(confpath)
            ),
            arguments(
                required<UUID::uuid>("id")
            ),
            internal::regen
        );

        cmd->subcommand(sub::regen::all(confpath));

        return cmd;
    }
}
