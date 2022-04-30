#include "database.h"

namespace minty::repo::db {
    auto database::read_posts(
        const std::vector<UUID::uuid>& posts
    ) -> std::vector<post_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<post_preview>>(
            tx,
            __FUNCTION__,
            posts
        );
    }
}
