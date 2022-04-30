#include "database.h"

namespace minty::repo::db {
    auto database::read_tag_sources(
        const UUID::uuid& tag_id
    ) -> std::vector<source> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<source>>(
            tx,
            __FUNCTION__,
            tag_id
        );
    }
}
