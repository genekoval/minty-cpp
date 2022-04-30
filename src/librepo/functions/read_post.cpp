#include "database.h"

namespace minty::repo::db {
    auto database::read_post(const UUID::uuid& post_id) -> post {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<post>(tx, __FUNCTION__, post_id);
    }
}
