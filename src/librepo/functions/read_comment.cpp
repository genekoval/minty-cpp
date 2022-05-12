#include "database.h"

namespace minty::repo::db {
    auto database::read_comment(const UUID::uuid& comment_id) -> comment {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return entix::make_entity<comment>(tx, __FUNCTION__, comment_id);
    }
}
