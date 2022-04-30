#include "database.h"

namespace minty::repo::db {
    auto database::create_source(
        std::string_view site_id,
        std::string_view resource
    ) -> source {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<source>(
            tx,
            __FUNCTION__,
            site_id,
            resource
        );
    }
}
