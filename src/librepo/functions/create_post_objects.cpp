
#include "database.h"

namespace minty::repo::db {
    auto database::create_post_objects(
        std::string_view post_id,
        const std::vector<std::string>& objects,
        unsigned int position
    ) -> post_object_update {
        auto tx = ntx();

        return {
            .objects = entix::make_entities<std::vector<object_preview>>(
                tx,
                __FUNCTION__,
                post_id,
                objects,
                position
            ),
            .date_modified = read_post_date_modified(tx, post_id)
        };
    }
}
