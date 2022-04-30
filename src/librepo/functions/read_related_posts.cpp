#include "database.h"

namespace minty::repo::db {
    auto database::read_related_posts(
        const UUID::uuid& post_id
    ) -> std::vector<post_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<post_preview>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }
}
