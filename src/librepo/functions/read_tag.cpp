#include "database.h"

namespace minty::repo::db {
    auto database::read_tag(const UUID::uuid& tag_id) -> tag {
        auto tx = ntx();

        try {
            return entix::make_entity<tag>(tx, __FUNCTION__, tag_id);
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw minty_error(
                "Tag with ID ({}) does not exist",
                tag_id
            );
        }
    }
}
