#include "database.h"

namespace minty::repo::db {
    auto database::prune_objects(
        const std::function<ext::task<bool>(
            std::span<const UUID::uuid>
        )>& on_deleted
    ) -> ext::task<> {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        const auto objects =
            entix::make_objects<std::vector<UUID::uuid>>(tx, __FUNCTION__);

        if (co_await on_deleted(objects)) tx.commit();
    }
}
