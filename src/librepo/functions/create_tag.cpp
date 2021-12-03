#include "database.h"

namespace minty::repo::db {
    auto database::create_tag(std::string_view name) -> std::string {
        return ntx()
            .exec_prepared1(__FUNCTION__, name)[0]
            .as<std::string>();
    }
}
