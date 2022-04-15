#include "database.h"

namespace minty::repo::db {
    auto database::prune_objects(
        std::function<bool(std::span<const UUID::uuid>)>&& on_deleted
    ) -> void {
        auto tx = pqxx::transaction(*connection);
        const auto objects =
            entix::make_objects<std::vector<UUID::uuid>>(tx, __FUNCTION__);

        if (on_deleted(objects)) tx.commit();
    }
}
