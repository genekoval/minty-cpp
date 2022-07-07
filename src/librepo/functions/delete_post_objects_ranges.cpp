#include "database.h"

namespace minty::repo::db {
    auto database::delete_post_objects_ranges(
        const UUID::uuid& post_id,
        std::span<const range> ranges
    ) -> decltype(post::date_modified) {
        auto arg = std::vector<std::string>();

        for (const auto& r : ranges) {
            arg.push_back(fmt::format(
                "[{},{}]",
                r.first + 1,
                r.last + 1
            ));
        }

        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(
            __FUNCTION__,
            post_id,
            arg
        )[0].as<decltype(post::date_modified)>();
    }
}
