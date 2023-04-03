#include "../repo/repo.hpp"
#include "commands.h"
#include "options/opts.h"

using namespace commline;

namespace {
    namespace internal {
        auto reindex(
            const app& app,
            std::string_view confpath
        ) -> void {
            const auto settings = minty::conf::initialize(confpath);

            minty::cli::repo(
                settings,
                [](minty::core::repo& repo) -> ext::task<> {
                    co_await repo.reindex();
                }
            );

            std::cout << "Minty search reindex complete" << std::endl;
        }
    }
}

namespace minty::cli {
    auto reindex(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "reindex",
            "Reindex the search engine",
            options(
                opts::config(confpath)
            ),
            arguments(),
            internal::reindex
        );
    }
}
