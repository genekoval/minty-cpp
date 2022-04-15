#include "database.h"

namespace minty::repo::db {
    auto database::update_object_preview(
        const UUID::uuid& object_id,
        const std::optional<UUID::uuid>& preview_id
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, object_id, preview_id);
    }
}
