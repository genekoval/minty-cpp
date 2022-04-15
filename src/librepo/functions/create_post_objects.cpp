
#include "database.h"

namespace minty::repo::db {
    auto database::create_post_objects(
        std::string_view post_id,
        const std::vector<UUID::uuid>& objects,
        std::int16_t position
    ) -> std::string {
        return ntx()
            .exec_prepared1(__FUNCTION__, post_id, objects, position)[0]
            .as<std::string>();
    }
}
