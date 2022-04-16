#include "database.h"

namespace minty::repo::db {
    auto database::create_post_tag(
        const UUID::uuid& post_id,
        std::string_view tag_id
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, post_id, tag_id);
    }
}
