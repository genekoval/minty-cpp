#include "database.h"

namespace minty::repo::db {
    auto database::create_comment(
        const UUID::uuid& post_id,
        std::string_view content
    ) -> comment {
        auto tx = ntx();

        return entix::make_entity<comment>(
            tx,
            __FUNCTION__,
            post_id,
            content
        );
    }
}
