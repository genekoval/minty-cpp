#include "database.h"

namespace minty::repo::db {
    auto database::delete_related_post(
        std::string_view post_id,
        std::string_view related
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, post_id, related);
    }
}
