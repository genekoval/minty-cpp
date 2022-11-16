#include "commands.h"
#include "../options/opts.h"
#include "../../api/api.h"

using namespace commline;

namespace {
    namespace internal {
        auto preview(
            const app& app,
            std::string_view confpath
        ) -> void {
            const auto settings = minty::conf::initialize(confpath);

            minty::cli::api(settings, [](auto& api) -> ext::task<> {
                const auto errors = api.get_object_preview_errors();

                for (const auto& error : errors) {
                    fmt::print("{}  {}\n", error.id, error.message);
                }

                fmt::print(
                    "{} error{}\n",
                    errors.size(),
                    errors.size() == 1 ? "" : "s"
                );

                co_return;
            });
        }
    }
}

namespace minty::cli::sub::errors {
    auto preview(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Read object preview error messages",
            options(
                opts::config(confpath)
            ),
            arguments(),
            internal::preview
        );
    }
}
