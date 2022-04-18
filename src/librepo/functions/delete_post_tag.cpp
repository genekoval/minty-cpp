#include "database.h"

namespace minty::repo::db {
    auto database::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, post_id, tag_id);
    }
}
