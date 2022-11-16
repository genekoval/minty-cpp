#include "database.h"

namespace minty::repo::db {
    auto database::read_objects(
        int batch_size
    ) -> ext::generator<std::span<object_preview>> {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        auto stream =
            entix::stream<object_preview>(tx, __FUNCTION__, batch_size);

        while (stream) co_yield stream();

        tx.commit();
    }
}
