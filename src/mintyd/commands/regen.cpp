#include "../api/api.h"
#include "commands.h"
#include "options/options.h"

namespace {
    auto $regen(
        const commline::app& app,
        const commline::argv& argv,
        std::string_view confpath
    ) -> void {
        if (argv.empty()) {
            throw commline::cli_error("No object ID given");
        }

        const auto settings = minty::conf::initialize(confpath);
        auto container = minty::cli::api_container(settings);

        const auto preview = container.api().regenerate_preview(argv.front());

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
    ) -> std::unique_ptr<commline::command_node> {
        return commline::command(
            __FUNCTION__,
            "Regenerate object previews",
            commline::options(options::config(confpath)),
            $regen
        );
    }
}
