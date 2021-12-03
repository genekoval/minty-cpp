#include "database.h"

namespace minty::repo::db {
    auto database::create_tag_alias(
        std::string_view tag_id,
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
