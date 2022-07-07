#include "database.h"

namespace minty::repo::db {
    auto database::move_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        const std::optional<UUID::uuid>& destination
    ) -> decltype(post::date_modified) {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(
            __FUNCTION__,
            post_id,
            objects,
            destination
        )[0].as<decltype(post::date_modified)>();
    }
}
