#include <minty/error.h>
#include <minty/repo/db/db.h>
#include <minty/repo/db/database.h>
#include <minty/repo/db/model.h>

namespace minty::repo::db {
    database::database(std::string_view connection_string) :
        connection(std::string(connection_string))
    {
        auto c = connection_initializer(connection);

        c.prepare("add_object", 1);
        c.prepare("create_site", 3);
        c.prepare("create_tag", 2);
        c.prepare("read_object", 1);
        c.prepare("update_object_preview", 2);
        c.prepare("update_object_source", 3);
    }

    auto database::add_object(std::string_view object_id) -> void {
        pqxx::nontransaction(connection).exec_prepared(
            "add_object",
            object_id
        );
    }

    auto database::create_site(
        std::string_view name,
        std::string_view homepage,
        std::optional<std::string_view> thumbnail_id
    ) -> site {
        auto tx = pqxx::nontransaction(connection);

        try {
            return make_entity<site>(
                tx,
                "create_site",
                name,
                homepage,
                thumbnail_id
            );
        }
        catch (const pqxx::unique_violation& ex) {
            throw unique_entity_violation("site", ex);
        }
    }

    auto database::create_tag(
        std::string_view name,
        std::string_view color
    ) -> tag {
        auto tx = pqxx::nontransaction(connection);

        try {
            return make_entity<tag>(tx, "create_tag", name, color);
        }
        catch (const pqxx::unique_violation& ex) {
            throw unique_entity_violation("tag", ex);
        }
    }

    auto database::read_object(
        std::string_view object_id
    ) -> object {
        auto tx = pqxx::nontransaction(connection);
        return make_entity<object>(tx, "read_object", object_id);
    }

    auto database::update_object_preview(
        std::string_view object_id,
        std::string_view preview_id
    ) -> void {
        pqxx::nontransaction(connection).exec_prepared(
            "update_object_preview",
            object_id,
            preview_id
        );
    }

    auto database::update_object_source(
        std::string_view object_id,
        const site& website,
        std::string_view url
    ) -> void {
        pqxx::nontransaction(connection).exec_prepared(
            "update_object_source",
            object_id,
            website.id,
            url
        );
    }
}
