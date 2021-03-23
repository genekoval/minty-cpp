#include <minty/error.h>
#include <minty/repo/db/database.h>
#include <minty/repo/db/parser.h>

namespace minty::repo::db {
    database::database(std::string_view connection_string) :
        connection(std::string(connection_string)),
        ntx(connection)
    {
        auto ci = connection_initializer(connection);

        ci.prepare("create_comment", 3);
        ci.prepare("create_creator", 1);
        ci.prepare("create_creator_aliases", 2);
        ci.prepare("create_creator_source", 3);
        ci.prepare("create_object", 3);
        ci.prepare("create_post", 5);
        ci.prepare("create_site", 3);
        ci.prepare("create_tag", 2);
        ci.prepare("read_comments", 1);
        ci.prepare("read_creator", 1);
        ci.prepare("read_creator_posts", 1);
        ci.prepare("read_creator_previews", 1);
        ci.prepare("read_creator_previews_all", 0);
        ci.prepare("read_object", 1);
        ci.prepare("read_objects", 1);
        ci.prepare("read_post", 1);
        ci.prepare("read_sources", 1);
        ci.prepare("read_tags", 1);
    }

    auto database::create_comment(
        std::string_view post_id,
        std::optional<std::string_view> parent_id,
        std::string_view content
    ) -> comment {
        return make_entity<comment>(
            ntx,
            "create_comment",
            post_id,
            parent_id,
            content
        );
    }

    auto database::create_creator(std::string_view name) -> std::string {
        return ntx
            .exec_prepared1("create_creator", name)[0]
            .as<std::string>();
    }

    auto database::create_creator_aliases(
        std::string_view creator_id,
        const std::vector<std::string>& aliases
    ) -> void {
        ntx.exec_prepared("create_creator_aliases", creator_id, aliases);
    }

    auto database::create_creator_source(
        std::string_view creator_id,
        std::string_view site_id,
        std::string_view url
    ) -> void {
        ntx.exec_prepared("create_creator_source", creator_id, site_id, url);
    }

    auto database::create_object(
        std::string_view object_id,
        std::optional<std::string_view> preview_id,
        std::optional<std::string_view> source_id
    ) -> void {
        ntx.exec_prepared("create_object", object_id, preview_id, source_id);
    }

    auto database::create_post(
        std::optional<std::string_view> title,
        std::optional<std::string_view> description,
        const std::vector<std::string>& objects,
        const std::vector<std::string>& creators,
        const std::vector<std::string>& tags
    ) -> std::string {
        return ntx.exec_prepared1(
            "create_post",
            title,
            description,
            objects,
            creators,
            tags
        )[0].as<std::string>();
    }

    auto database::create_site(
        std::string_view name,
        std::string_view homepage,
        std::optional<std::string_view> thumbnail_id
    ) -> site {
        try {
            return make_entity<site>(
                ntx,
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
        try {
            return make_entity<tag>(ntx, "create_tag", name, color);
        }
        catch (const pqxx::unique_violation& ex) {
            throw unique_entity_violation("tag", ex);
        }
    }

    auto database::read_comments(
        std::string_view post_id
    ) -> std::vector<comment> {
        return make_entities<std::vector<comment>>(
            ntx,
            "read_comments",
            post_id
        );
    }

    auto database::read_creator(std::string_view creator_id) -> creator {
        try {
            return make_entity<creator>(ntx, "read_creator", creator_id);
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw minty_error(
                "Creator with ID ({}) does not exist",
                creator_id
            );
        }
    }

    auto database::read_creator_posts(
        std::string_view creator_id
    ) -> std::vector<post_preview> {
        return make_entities<std::vector<post_preview>>(
            ntx,
            "read_creator_posts",
            creator_id
        );
    }

    auto database::read_creator_previews_all() -> std::vector<creator_preview> {
        return make_entities<std::vector<creator_preview>>(
            ntx,
            "read_creator_previews_all"
        );
    }

    auto database::read_object(std::string_view object_id) -> object {
        return make_entity<object>(ntx, "read_object", object_id);
    }

    auto database::read_objects(
        std::string_view post_id
    ) -> std::vector<object> {
        return make_entities<std::vector<object>>(
            ntx,
            "read_objects",
            post_id
        );
    }

    auto database::read_post(std::string_view post_id) -> post {
        return make_entity<post>(ntx, "read_post", post_id);
    }
}
