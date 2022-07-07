#include "database.h"

namespace minty::repo::db {
    auto database::delete_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects
    ) -> decltype(post::date_modified) {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(
            __FUNCTION__,
            post_id,
            objects
        )[0].as<decltype(post::date_modified)>();
    }
}
