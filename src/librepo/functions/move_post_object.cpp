#include "database.h"

namespace minty::repo::db {
    auto database::move_post_object(
        const UUID::uuid& post_id,
        unsigned int old_index,
        unsigned int new_index
    ) -> std::string {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(
            __FUNCTION__,
            post_id,
            old_index + 1,
            new_index + 1
        )[0].as<std::string>();
    }
}
