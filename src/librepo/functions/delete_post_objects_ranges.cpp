#include "database.h"

namespace minty::repo::db {
    auto database::delete_post_objects_ranges(
        std::string_view post_id,
        std::span<const range> ranges
    ) -> std::string {
        auto arg = std::vector<std::string>();

        for (const auto& r : ranges) {
            arg.push_back(fmt::format(
                "[{},{}]",
                r.first + 1,
                r.last + 1
            ));
        }

        return ntx().exec_prepared1(
            __FUNCTION__,
            post_id,
            arg
        )[0].as<std::string>();
    }
}
