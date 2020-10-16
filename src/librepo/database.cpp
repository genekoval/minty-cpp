#include <minty/error.h>
#include <minty/repo/db/db.h>
#include <minty/repo/db/database.h>
#include <minty/repo/db/model.h>

namespace minty::repo::db {
    database::database(std::string_view connection_string) :
        connection(std::string(connection_string))
    {
        auto c = connection_initializer(connection);

        c.prepare("add_object", 4);
        c.prepare("create_site", 3);
        c.prepare("create_tag", 2);
    }

    auto database::add_object(
        std::string_view object_id,
        std::optional<std::string_view> preview_id,
        std::optional<data::source> src
    ) -> object {
        auto tx = pqxx::nontransaction(connection);

        return make_entity<object>(
            tx,
            "add_object",
            object_id,
            preview_id,
            src.has_value() ?
                std::make_optional(src->website->id) :
                std::optional<std::remove_const_t<decltype(site::id)>>(),
            src.has_value() ?
                std::make_optional(src->url) :
                std::optional<decltype(source::url)>()
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
}
