#include "database.h"

namespace minty::repo::db {
    auto database::delete_tag(const UUID::uuid& tag_id) -> void {
        ntx().exec_prepared(__FUNCTION__, tag_id);
    }
}
