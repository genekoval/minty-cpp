#include "database.h"

namespace minty::repo::db {
    auto database::read_post_objects(
        const UUID::uuid& post_id
    ) -> std::vector<object_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<object_preview>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }
}
