#include "database.h"

namespace minty::repo::db {
    auto database::create_reply(
        std::string_view parent_id,
        std::string_view content
    ) -> comment {
        auto tx = ntx();

        return entix::make_entity<comment>(
            tx,
            __FUNCTION__,
            parent_id,
            content
        );
    }
}
