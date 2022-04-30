#include "database.h"

namespace minty::repo::db {
    auto database::create_post(
        std::string_view title,
        std::string_view description,
        const std::vector<UUID::uuid>& objects,
        const std::vector<UUID::uuid>& tags
    ) -> post_search {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return entix::make_entity<post_search>(
            tx,
            __FUNCTION__,
            title,
            description,
            objects,
            tags
        );
    }
}
