#include "database.h"

namespace minty::repo::db {
    auto database::update_post_title(
        const UUID::uuid& post_id,
        std::string_view title
    ) -> post_update {
        auto tx = ntx();
        return entix::make_entity<post_update>(
            tx,
            __FUNCTION__,
            post_id,
            title
        );
    }
}
