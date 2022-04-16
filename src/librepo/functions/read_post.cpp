#include "database.h"

namespace minty::repo::db {
    auto database::read_post(const UUID::uuid& post_id) -> post {
        auto tx = ntx();
        return entix::make_entity<post>(tx, __FUNCTION__, post_id);
    }
}
