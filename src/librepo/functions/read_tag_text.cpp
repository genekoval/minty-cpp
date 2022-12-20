#include "database.h"

namespace minty::repo::db {
    auto database::read_tag_text(
        int batch_size
    ) -> ext::generator<std::span<tag_search>> {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        auto stream = entix::stream<tag_search>(tx, __FUNCTION__, batch_size);

        while (stream) co_yield stream();

        tx.commit();
    }
}
