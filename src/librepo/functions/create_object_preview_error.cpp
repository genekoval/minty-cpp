#include "database.h"

namespace minty::repo::db {
    auto database::create_object_preview_error(
        const UUID::uuid& object_id,
        std::string_view message
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, object_id, message);
    }
}
