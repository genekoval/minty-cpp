#include "database.h"

namespace minty::repo::db {
    auto database::read_total_objects() -> std::size_t {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(__FUNCTION__)[0].as<std::size_t>();
    }
}
