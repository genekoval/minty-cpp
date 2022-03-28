#include "cli.h"

#include "../../client.h"
#include "../../output.h"

namespace minty::cli {
    auto print_tag(
        minty::api& api,
        std::string_view id,
        std::optional<std::string_view> path
    ) -> void {
        cli::print(api.get_tag(id), path);
    }
}
