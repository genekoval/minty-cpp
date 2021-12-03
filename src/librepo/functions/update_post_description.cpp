#include "database.h"

namespace minty::repo::db {
    auto database::update_post_description(
        std::string_view post_id,
        std::string_view description
    ) -> post_update {
        auto tx = ntx();
        return entix::make_entity<post_update>(
            tx,
            __FUNCTION__,
            post_id,
            description
        );
    }
}
