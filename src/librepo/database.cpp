#include "db.h"

#include <minty/repo/db/database.h>
#include <minty/repo/db/model.h>

namespace minty::repo::db {
    database::database(std::string_view connection_string) :
        connection(std::string(connection_string))
    {
        auto c = connection_initializer(connection);

        c.prepare("create_tag", 2);
    }

    auto database::create_tag(
        std::string_view name,
        std::string_view color
    ) -> model::tag {
        auto tx = pqxx::nontransaction(connection);

        return tag(tx.exec_prepared1("create_tag", name, color));
    }
}
