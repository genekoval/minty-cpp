#include "database.h"

namespace minty::repo::db {
    auto database::update_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> post_update {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<post_update>(
            tx,
            __FUNCTION__,
            post_id,
            description
        );
    }
}
