#include "database.h"

namespace minty::repo::db {
    auto database::read_tag_text(
        int batch_size,
        std::function<void(std::span<const tag_text>)>&& action
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        entix::stream(tx, __FUNCTION__, batch_size, action);
        tx.commit();
    }
}
