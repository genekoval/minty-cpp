#include "database.h"

namespace minty::repo::db {
    auto database::update_comment(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, comment_id, content);
    }
}
