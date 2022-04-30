#include "database.h"

namespace minty::repo::db {
    auto database::create_object(
        const UUID::uuid& object_id,
        const std::optional<UUID::uuid>& preview_id,
        const std::optional<std::string_view>& source_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, object_id, preview_id, source_id);
    }
}
