#include "database.h"

namespace minty::repo::db {
    auto database::read_tag_previews(
        const std::vector<UUID::uuid>& tags
    ) -> std::vector<tag_preview> {
        auto tx = ntx();
        return entix::make_entities<std::vector<tag_preview>>(
            tx,
            __FUNCTION__,
            tags
        );
    }
}
