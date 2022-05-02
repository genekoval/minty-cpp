#include "database.h"

namespace minty::repo::db {
    auto database::read_post_search(
        int batch_size,
        std::function<void(std::span<const post_search>)>&& action
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        entix::stream(tx, __FUNCTION__, batch_size, action);
        tx.commit();
    }
}
