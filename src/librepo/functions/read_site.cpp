#include "database.h"

namespace minty::repo::db {
    auto database::read_site(
        std::string_view scheme,
        std::string_view host
    ) -> std::optional<std::string> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx
            .exec_prepared1(__FUNCTION__, scheme, host)[0]
            .as<std::optional<std::string>>();
    }
}
