#include "database.h"

namespace minty::repo::db {
    auto database::delete_object_preview_error(
        const UUID::uuid& object_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, object_id);
    }
}
