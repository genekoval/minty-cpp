#include "database.h"

namespace minty::repo::db {
    auto database::create_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, post_id, related);
    }
}
