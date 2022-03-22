#include "database.h"

namespace minty::repo::db {
    auto database::read_related_posts(
        std::string_view post_id
    ) -> std::vector<post_preview> {
        auto tx = ntx();
        return entix::make_entities<std::vector<post_preview>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }
}
