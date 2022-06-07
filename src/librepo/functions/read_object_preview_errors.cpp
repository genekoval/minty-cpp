#include "database.h"

namespace minty::repo::db {
    auto database::read_object_preview_errors() -> std::vector<object_error> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return entix::make_entities<std::vector<object_error>>(
            tx,
            __FUNCTION__
        );
    }
}
