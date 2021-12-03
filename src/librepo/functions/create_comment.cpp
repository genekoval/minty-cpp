#include "database.h"

namespace minty::repo::db {
    auto database::create_comment(
        std::string_view post_id,
        std::optional<std::string_view> parent_id,
        std::string_view content
    ) -> comment {
        auto tx = ntx();

        return entix::make_entity<comment>(
            tx,
            __FUNCTION__,
            post_id,
            parent_id,
            content
        );
    }
}
