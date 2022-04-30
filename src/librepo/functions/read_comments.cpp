#include "database.h"

namespace minty::repo::db {
    auto database::read_comments(
        const UUID::uuid& post_id
    ) -> std::vector<comment> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<comment>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }
}
