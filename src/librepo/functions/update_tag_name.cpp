#include "database.h"

namespace minty::repo::db {
    auto database::update_tag_name(
        const UUID::uuid& tag_id,
        std::string_view name
    ) -> tag_name_update {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<tag_name_update>(
            tx,
            __FUNCTION__,
            tag_id,
            name
        );
    }
}
