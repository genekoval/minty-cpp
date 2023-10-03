#include "../repo/repo.hpp"
#include "commands.h"
#include "options/opts.h"

#include <fmt/color.h>

using namespace commline;

using fmt::color;
using fmt::fg;

namespace {
    auto on_error(std::span<const std::string> errors) -> void {
        for (const auto& error : errors) fmt::print("\n{}", error);
    }

    auto reindex(std::string_view label, ext::task<bool> task) -> ext::task<> {
        fmt::print("Reindexing {}...", label);
        fflush(stdout);

        const auto no_errors = co_await task;

        if (!no_errors) fmt::print("\n");
        fmt::print(fg(color::lime_green), "done\n");
    }

    auto reindex_posts(minty::core::repo& repo, unsigned int batch_size)
        -> ext::task<> {
        co_await reindex("posts", repo.reindex_posts(batch_size, on_error));
    }

    auto reindex_tags(minty::core::repo& repo, unsigned int batch_size)
        -> ext::task<> {
        co_await reindex("tags", repo.reindex_tags(batch_size, on_error));
    }

    namespace internal {
        auto reindex_posts(
            const app& app,
            unsigned int batch_size,
            std::string_view confpath
        ) -> void {
            minty::cli::repo(
                minty::conf::initialize(confpath),
                [batch_size](minty::core::repo& repo) -> ext::task<> {
                    co_await ::reindex_posts(repo, batch_size);
                }
            );
        }

        auto reindex_tags(
            const app& app,
            unsigned int batch_size,
            std::string_view confpath
        ) -> void {
            minty::cli::repo(
                minty::conf::initialize(confpath),
                [batch_size](minty::core::repo& repo) -> ext::task<> {
                    co_await ::reindex_tags(repo, batch_size);
                }
            );
        }

        auto reindex(
            const app& app,
            unsigned int batch_size,
            std::string_view confpath
        ) -> void {
            minty::cli::repo(
                minty::conf::initialize(confpath),
                [batch_size](minty::core::repo& repo) -> ext::task<> {
                    co_await ::reindex_tags(repo, batch_size);
                    co_await ::reindex_posts(repo, batch_size);
                }
            );
        }
    }
}

namespace minty::cli {
    auto batch_size() -> option<unsigned int> {
        return option<unsigned int>(
            {"b", "batch-size"},
            "Number of items to transfer at a time",
            "size",
            500
        );
    }

    auto reindex(std::string_view confpath) -> std::unique_ptr<command_node> {
        auto cmd = command(
            "reindex",
            "Reindex the search engine",
            options(batch_size(), opts::config(confpath)),
            arguments(),
            internal::reindex
        );

        cmd->subcommand(command(
            "posts",
            "Reindex posts",
            options(batch_size(), opts::config(confpath)),
            arguments(),
            internal::reindex_posts
        ));

        cmd->subcommand(command(
            "tags",
            "Reindex tags",
            options(batch_size(), opts::config(confpath)),
            arguments(),
            internal::reindex_tags
        ));

        return cmd;
    }
}
