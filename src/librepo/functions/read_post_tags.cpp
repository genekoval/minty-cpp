#include "database.h"

namespace minty::repo::db {
    auto database::read_post_tags(
        std::string_view post_id
    ) -> std::vector<tag_preview> {
        auto tx = ntx();
        return entix::make_entities<std::vector<tag_preview>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }
}
