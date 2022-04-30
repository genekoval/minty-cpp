#include "database.h"

namespace minty::repo::db {
    auto database::create_tag(std::string_view name) -> UUID::uuid {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx
            .exec_prepared1(__FUNCTION__, name)[0]
            .as<UUID::uuid>();
    }
}
