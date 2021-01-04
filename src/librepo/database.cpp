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
        c.prepare("create_comment", 3);
        c.prepare("create_creator", 1);
        c.prepare("create_creator_aliases", 2);
        c.prepare("create_creator_source", 3);
        c.prepare("create_site", 3);
        c.prepare("create_tag", 2);
        c.prepare("create_post", 4);
        c.prepare("read_creator", 1);
        c.prepare("read_creator_previews", 1);
        c.prepare("read_object", 1);
        c.prepare("read_objects", 1);
        c.prepare("read_post", 1);
        c.prepare("read_sources", 1);
        c.prepare("read_tags", 1);
        c.prepare("update_object_preview", 2);
        c.prepare("update_object_source", 3);
    }

    auto database::add_object(std::string_view object_id) -> void {
        pqxx::nontransaction(connection).exec_prepared(
            "add_object",
            object_id
        );
    }

    auto database::create_comment(
        std::string_view post_id,
        std::optional<std::string_view> parent_id,
        std::string_view content
    ) -> comment {
        auto tx = pqxx::nontransaction(connection);
        return make_entity<comment>(
            tx,
            "create_comment",
            post_id,
            parent_id,
            content
        );
    }

    auto database::create_creator(
        std::string_view name
    ) -> std::string {
        return pqxx::nontransaction(connection)
            .exec_prepared1("create_creator", name)[0]
            .as<std::string>();
    }

    auto database::create_creator_aliases(
        std::string_view creator_id,
        const std::vector<std::string>& aliases
    ) -> void {
        pqxx::nontransaction(connection)
            .exec_prepared("create_creator_aliases", creator_id, aliases);
    }

    auto database::create_creator_source(
        std::string_view creator_id,
        std::string_view site_id,
        std::string_view url
    ) -> void {
        pqxx::nontransaction(connection)
            .exec_prepared("create_creator_source", creator_id, site_id, url);
    }

    auto database::create_post(
        std::string_view description,
        const std::vector<std::string>& objects,
        std::optional<std::string_view> creator_id,
        const std::vector<std::string>& tags
    ) -> std::string {
        return pqxx::nontransaction(connection).exec_prepared1(
            "create_post",
            description,
            objects,
            creator_id,
            tags
        )[0].as<std::string>();
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

    auto database::read_creator(std::string_view creator_id) -> creator {
        auto tx = pqxx::nontransaction(connection);
        return make_entity<creator>(tx, "read_creator", creator_id);
    }

    auto database::read_object(std::string_view object_id) -> object {
        auto tx = pqxx::nontransaction(connection);
        return make_entity<object>(tx, "read_object", object_id);
    }

    auto database::read_post(std::string_view post_id) -> post {
        auto tx = pqxx::nontransaction(connection);
        return make_entity<post>(tx, "read_post", post_id);
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
