#include "database.h"

namespace minty::repo::db {
    auto database::delete_post_objects(
        std::string_view post_id,
        const std::vector<UUID::uuid>& objects
    ) -> std::string {
        return ntx().exec_prepared1(
            __FUNCTION__,
            post_id,
            objects
        )[0].as<std::string>();
    }
}
