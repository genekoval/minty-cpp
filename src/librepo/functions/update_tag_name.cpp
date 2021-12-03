#include "database.h"

namespace minty::repo::db {
    auto database::update_tag_name(
        std::string_view tag_id,
        std::string_view name
    ) -> tag_name_update {
        auto tx = ntx();
        return entix::make_entity<tag_name_update>(
            tx,
            __FUNCTION__,
            tag_id,
            name
        );
    }
}
