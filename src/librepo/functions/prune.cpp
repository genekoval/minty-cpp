#include "database.h"

namespace minty::repo::db {
    auto database::prune() -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__);
    }
}
