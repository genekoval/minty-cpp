#include "../repo/repo.hpp"
#include "commands.h"
#include "options/opts.h"

#include <fmt/format.h>

using namespace commline;

namespace {
    namespace internal {
        auto prune(
            const app& app,
            std::string_view confpath
        ) -> void {
            const auto settings = minty::conf::initialize(confpath);

            minty::cli::repo(
                settings,
                [](minty::core::repo& repo) -> ext::task<> {
                    co_await repo.prune();
                }
            );
        }
    }
}

namespace minty::cli {
    auto prune(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "prune",
            "Delete unused data",
            options(
                opts::config(confpath)
            ),
            arguments(),
            internal::prune
        );
    }
}
