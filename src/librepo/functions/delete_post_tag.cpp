#include "database.h"

namespace minty::repo::db {
    auto database::delete_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, post_id, tag_id);
    }
}
