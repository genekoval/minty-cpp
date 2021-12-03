#include "database.h"

namespace minty::repo::db {
    auto database::read_tag_sources(
        std::string_view tag_id
    ) -> std::vector<source> {
        auto tx = ntx();
        return entix::make_entities<std::vector<source>>(
            tx,
            __FUNCTION__,
            tag_id
        );
    }
}
