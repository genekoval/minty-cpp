#include "database.h"

namespace minty::repo::db {
    auto database::read_objects(
        int batch_size,
        std::function<void(std::span<const object_preview>)>&& action
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        entix::stream(tx, __FUNCTION__, batch_size, action);
        tx.commit();
    }
}
