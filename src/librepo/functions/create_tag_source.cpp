#include "database.h"

namespace minty::repo::db {
    auto database::create_tag_source(
        const UUID::uuid& tag_id,
        std::string_view source_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, tag_id, source_id);
    }
}
