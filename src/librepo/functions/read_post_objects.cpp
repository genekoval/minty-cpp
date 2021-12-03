#include "database.h"

namespace minty::repo::db {
    auto database::read_post_objects(
        std::string_view post_id
    ) -> std::vector<object_preview> {
        auto tx = ntx();
        return entix::make_entities<std::vector<object_preview>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }
}
