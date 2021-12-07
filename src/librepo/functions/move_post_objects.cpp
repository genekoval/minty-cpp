#include "database.h"

namespace minty::repo::db {
    auto database::move_post_objects(
        std::string_view post_id,
        const std::vector<std::string>& objects,
        std::optional<std::string> destination
    ) -> std::string {
        return ntx().exec_prepared1(
            __FUNCTION__,
            post_id,
            objects,
            destination
        )[0].as<std::string>();
    }
}
