#include "commands.h"
#include "../../client.h"

#include <fstream>

using namespace commline;

namespace fs = std::filesystem;

namespace {
    namespace internal {
        auto get(
            const app& app,
            bool no_clobber,
            std::string_view id,
            std::optional<std::string_view> file
        ) -> void {
            auto bucket = minty::cli::bucket();

            if (!file) {
                bucket.get(id, std::cout);
                return;
            }

            auto path = fs::path(*file);

            if (fs::is_directory(path)) path = path / id;
            if (no_clobber && fs::exists(path)) return;

            auto out = std::ofstream(path);
            bucket.get(id, out);
        }
    }
}

namespace minty::subcommands::object {
    auto get() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Download an object's data",
            options(
                flag({"n", "no-clobber"}, "Do not overwrite an existing file")
            ),
            arguments(
                required<std::string_view>("id"),
                optional<std::string_view>("file")
            ),
            internal::get
        );
    }
}
