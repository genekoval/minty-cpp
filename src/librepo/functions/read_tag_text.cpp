#include "database.h"

namespace minty::repo::db {
    auto database::read_tag_text() -> std::vector<tag_text> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<tag_text>>(tx, __FUNCTION__);
    }
}
