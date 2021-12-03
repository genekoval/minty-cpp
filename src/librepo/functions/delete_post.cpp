#include "database.h"

namespace minty::repo::db {
    auto database::delete_post(std::string_view post_id) -> void {
        ntx().exec_prepared(__FUNCTION__, post_id);
    }
}
