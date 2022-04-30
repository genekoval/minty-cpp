#include "database.h"

namespace minty::repo::db {
    auto database::create_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> tag_name {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<tag_name>(
            tx,
            __FUNCTION__,
            tag_id,
            alias
        );
    }
}
