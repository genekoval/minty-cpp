#include "database.h"

namespace minty::repo::db {
    auto database::update_object_preview(
        std::string_view object_id,
        const std::optional<std::string>& preview_id
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, object_id, preview_id);
    }
}
