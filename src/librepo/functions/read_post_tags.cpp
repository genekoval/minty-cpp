#include "database.h"

namespace minty::repo::db {
    auto database::read_post_tags(
        const UUID::uuid& post_id
    ) -> std::vector<tag_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<tag_preview>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }
}
