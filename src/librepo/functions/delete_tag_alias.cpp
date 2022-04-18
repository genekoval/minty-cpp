#include "database.h"

namespace minty::repo::db {
    auto database::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> tag_name {
        auto tx = ntx();
        return entix::make_entity<tag_name>(
            tx,
            __FUNCTION__,
            tag_id,
            alias
        );
    }
}
