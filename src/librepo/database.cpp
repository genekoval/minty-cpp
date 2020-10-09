#include "db.h"

#include <minty/error.h>
#include <minty/repo/db/database.h>
#include <minty/repo/db/model.h>

namespace minty::repo::db {
    database::database(std::string_view connection_string) :
        connection(std::string(connection_string))
    {
        auto c = connection_initializer(connection);

        c.prepare("create_site", 3);
        c.prepare("create_tag", 2);
    }

    auto database::create_site(
        std::string_view name,
        std::string_view homepage,
        std::optional<std::string_view> thumbnail_id
    ) -> model::site {
        auto tx = pqxx::nontransaction(connection);

        try {
            return site(tx.exec_prepared1(
                "create_site",
                name,
                homepage,
                thumbnail_id
            ));
        }
        catch (const pqxx::unique_violation& ex) {
            throw unique_entity_violation(site::entity, ex);
        }
    }

    auto database::create_tag(
        std::string_view name,
        std::string_view color
    ) -> model::tag {
        auto tx = pqxx::nontransaction(connection);

        try {
            return tag(tx.exec_prepared1("create_tag", name, color));
        }
        catch (const pqxx::unique_violation& ex) {
            throw unique_entity_violation(tag::entity, ex);
        }
    }
}
