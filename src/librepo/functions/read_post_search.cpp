#include "database.h"

namespace minty::repo::db {
    auto database::read_post_search(
        int batch_size
    ) -> ext::generator<std::span<post_search>> {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        auto stream = entix::stream<post_search>(tx, __FUNCTION__, batch_size);

        while (stream) co_yield stream();

        tx.commit();
    }
}
