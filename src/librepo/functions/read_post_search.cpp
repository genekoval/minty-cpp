#include "database.h"

namespace minty::repo::db {
    auto database::read_post_search() -> std::vector<post_search> {
        auto tx = ntx();
        return entix::make_entities<std::vector<post_search>>(tx, __FUNCTION__);
    }
}
