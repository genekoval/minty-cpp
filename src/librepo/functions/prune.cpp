#include "database.h"

namespace minty::repo::db {
    auto database::prune() -> void {
        ntx().exec_prepared(__FUNCTION__);
    }
}
