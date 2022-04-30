#include "database.h"

namespace minty::repo::db {
    auto database::create_site(
        std::string_view scheme,
        std::string_view name,
        std::optional<UUID::uuid> icon
    ) -> site {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        try {
            return entix::make_entity<site>(
                tx,
                __FUNCTION__,
                scheme,
                name,
                icon
            );
        }
        catch (const pqxx::unique_violation& ex) {
            throw unique_entity_violation("site", ex);
        }
    }
}
