#include "../api/api.h"
#include "commands.h"
#include "options/opts.h"

#include <uuid++/commline>

using namespace commline;

namespace {
    auto $regen(
        const commline::app& app,
        std::string_view confpath,
        const UUID::uuid& id
    ) -> void {
        const auto settings = minty::conf::initialize(confpath);
        auto container = minty::cli::api_container(settings);

        const auto preview = container.api().regenerate_preview(id);

        if (preview) {
            std::cout << "Preview ID: " << *preview << std::endl;
        }
        else {
            std::cout << "Preview could not be generated" << std::endl;
        }
    }
}

namespace minty::cli {
    auto regen(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Regenerate object previews",
            options(
                opts::config(confpath)
            ),
            arguments(
                required<UUID::uuid>("id")
            ),
            $regen
        );
    }
}
