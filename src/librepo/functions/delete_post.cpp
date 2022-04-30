#include "database.h"

namespace minty::repo::db {
    auto database::delete_post(const UUID::uuid& post_id) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, post_id);
    }
}
