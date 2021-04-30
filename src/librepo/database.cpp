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
        ci.prepare("create_object", 3);
        ci.prepare("create_post", 4);
        ci.prepare("create_site", 3);

        ci.prepare("create_tag", 1);
        ci.prepare("create_tag_alias", 2);
        ci.prepare("create_tag_source", 3);

        ci.prepare("delete_post", 1);
        ci.prepare("delete_tag", 1);
        ci.prepare("delete_tag_alias", 2);
        ci.prepare("delete_tag_source", 2);

        ci.prepare("read_comments", 1);
        ci.prepare("read_object", 1);
        ci.prepare("read_objects", 1);
        ci.prepare("read_post", 1);
        ci.prepare("read_post_tags", 1);
        ci.prepare("read_site", 2);
        ci.prepare("read_sources", 1);
        ci.prepare("read_tag", 1);
        ci.prepare("read_tag_posts", 1);
        ci.prepare("read_tag_previews", 1);
        ci.prepare("read_tag_previews_all", 0);

        ci.prepare("update_tag_description", 2);
        ci.prepare("update_tag_name", 2);
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
        const std::vector<std::string>& tags
    ) -> std::string {
        return ntx.exec_prepared1(
            "create_post",
            title,
            description,
            objects,
            tags
        )[0].as<std::string>();
    }

    auto database::create_site(
        std::string_view scheme,
        std::string_view name,
        std::optional<std::string_view> icon
    ) -> site {
        try {
            return make_entity<site>(
                ntx,
                "create_site",
                scheme,
                name,
                icon
            );
        }
        catch (const pqxx::unique_violation& ex) {
            throw unique_entity_violation("site", ex);
        }
    }

    auto database::create_tag(std::string_view name) -> std::string {
        return ntx
            .exec_prepared1("create_tag", name)[0]
            .as<std::string>();
    }

    auto database::create_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        return make_entity<tag_name>(ntx, "create_tag_alias", tag_id, alias);
    }

    auto database::create_tag_source(
        std::string_view tag_id,
        std::string_view site_id,
        std::string_view resource
    ) -> source {
        return make_entity<source>(
            ntx,
            "create_tag_source",
            tag_id,
            site_id,
            resource
        );
    }

    auto database::delete_post(std::string_view post_id) -> void {
        ntx.exec_prepared("delete_post", post_id);
    }

    auto database::delete_tag(std::string_view tag_id) -> void {
        ntx.exec_prepared("delete_tag", tag_id);
    }

    auto database::delete_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        return make_entity<tag_name>(ntx, "delete_tag_alias", tag_id, alias);
    }

    auto database::delete_tag_source(
        std::string_view tag_id,
        std::string_view source_id
    ) -> void {
        ntx.exec_prepared("delete_tag_source", tag_id, source_id);
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

    auto database::read_post_tags(
        std::string_view post_id
    ) -> std::vector<tag_preview> {
        return make_entities<std::vector<tag_preview>>(
            ntx,
            "read_post_tags",
            post_id
        );
    }

    auto database::read_site(
        std::string_view scheme,
        std::string_view host
    ) -> std::optional<std::string> {
        return ntx
            .exec_prepared1("read_site", scheme, host)[0]
            .as<std::optional<std::string>>();
    }

    auto database::read_tag(std::string_view tag_id) -> tag {
        try {
            return make_entity<tag>(ntx, "read_tag", tag_id);
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw minty_error(
                "Tag with ID ({}) does not exist",
                tag_id
            );
        }
    }

    auto database::read_tag_posts(
        std::string_view tag_id
    ) -> std::vector<post_preview> {
        return make_entities<std::vector<post_preview>>(
            ntx,
            "read_tag_posts",
            tag_id
        );
    }

    auto database::read_tag_previews(
        const std::vector<std::string>& tags
    ) -> std::vector<tag_preview> {
        return make_entities<std::vector<tag_preview>>(
            ntx,
            "read_tag_previews",
            tags
        );
    }

    auto database::read_tag_previews_all() -> std::vector<tag_preview> {
        return make_entities<std::vector<tag_preview>>(
            ntx,
            "read_tag_previews_all"
        );
    }

    auto database::update_tag_description(
        std::string_view tag_id,
        std::string_view description
    ) -> std::optional<std::string> {
        return ntx
            .exec_prepared1("update_tag_description", tag_id, description)[0]
            .as<std::optional<std::string>>();
    }

    auto database::update_tag_name(
        std::string_view tag_id,
        std::string_view name
    ) -> tag_name {
        return make_entity<tag_name>(ntx, "update_tag_name", tag_id, name);
    }
}
