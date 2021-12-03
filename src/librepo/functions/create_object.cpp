#include "database.h"

namespace minty::repo::db {
    auto database::create_object(
        std::string_view object_id,
        std::optional<std::string_view> preview_id,
        std::optional<std::string_view> source_id
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, object_id, preview_id, source_id);
    }
}
