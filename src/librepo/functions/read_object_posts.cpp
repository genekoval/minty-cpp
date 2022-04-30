#include "database.h"

namespace minty::repo::db {
    auto database::read_object_posts(
        const UUID::uuid& object_id
    ) -> std::vector<post_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<post_preview>>(
            tx,
            __FUNCTION__,
            object_id
        );
    }
}
