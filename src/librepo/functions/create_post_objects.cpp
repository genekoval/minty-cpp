
#include "database.h"

namespace minty::repo::db {
    auto database::create_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        std::int16_t position
    ) -> std::string {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx
            .exec_prepared1(__FUNCTION__, post_id, objects, position)[0]
            .as<std::string>();
    }
}
