#include "database.h"

namespace minty::repo::db {
    auto database::read_comments(
        std::string_view post_id
    ) -> std::vector<comment> {
        auto tx = ntx();
        return entix::make_entities<std::vector<comment>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }
}
