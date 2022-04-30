#include "database.h"

namespace minty::repo::db {
    auto database::update_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> std::optional<std::string> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx
            .exec_prepared1(__FUNCTION__, tag_id, description)[0]
            .as<std::optional<std::string>>();
    }
}
