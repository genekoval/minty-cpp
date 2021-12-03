#include "database.h"

namespace minty::repo::db {
    auto database::read_object(std::string_view object_id) -> object {
        auto tx = ntx();
        return entix::make_entity<object>(tx, __FUNCTION__, object_id);
    }
}
