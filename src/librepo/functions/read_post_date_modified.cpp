#include "database.h"

namespace minty::repo::db {
    auto database::read_post_date_modified(
        pqxx::transaction_base& tx,
        std::string_view post_id
    ) -> std::string {
        return tx.exec_prepared1(__FUNCTION__, post_id)[0].as<std::string>();
    }
}
