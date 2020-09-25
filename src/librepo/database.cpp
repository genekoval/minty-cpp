#include <minty/repo/db/database.h>
#include <minty/repo/db/model.h>

namespace minty::repo::db {
    database::database(std::string_view connection_string) :
        connection(std::string(connection_string))
    {}

    auto database::create_tag(
        std::string_view name,
        std::string_view color
    ) -> model::tag {
        auto tx = pqxx::nontransaction(connection);

        return tag(tx.exec_params1(R"(
            SELECT name, color, date_created
            FROM create_tag($1, $2)
        )",
            name,
            color
        ));
    }
}
