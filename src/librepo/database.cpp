#include <minty/error.h>
#include <minty/repo/db/database.h>
#include <minty/repo/db/db.h>
#include <minty/repo/db/entity.h>

#include <fmt/format.h>

namespace minty::repo::db {
    database::database(std::string_view connection_string) :
        connection(std::string(connection_string))
    {
        auto c = entix::connection(connection);

        c.prepare("create_comment", 3);
        c.prepare("create_object", 3);
        c.prepare("create_post", 4);
        c.prepare("create_post_objects", 3);
        c.prepare("create_post_tag", 2);
        c.prepare("create_site", 3);
        c.prepare("create_source", 2);

        c.prepare("create_tag", 1);
        c.prepare("create_tag_alias", 2);
        c.prepare("create_tag_source", 2);

        c.prepare("delete_post", 1);
        c.prepare("delete_post_objects", 2);
        c.prepare("delete_post_tag", 2);
        c.prepare("delete_tag", 1);
        c.prepare("delete_tag_alias", 2);
        c.prepare("delete_tag_source", 2);

        c.prepare("move_post_object", 3);

        c.prepare("prune", 0);
        c.prepare("prune_objects", 0);

        c.prepare("read_comments", 1);
        c.prepare("read_object", 1);
        c.prepare("read_object_posts", 1);
        c.prepare("read_post", 1);
        c.prepare("read_post_date_modified", 1);
        c.prepare("read_posts", 1);
        c.prepare("read_post_objects", 1);
        c.prepare("read_post_search", 0);
        c.prepare("read_post_tags", 1);
        c.prepare("read_site", 2);
        c.prepare("read_tag", 1);
        c.prepare("read_tag_previews", 1);
        c.prepare("read_tag_sources", 1);
        c.prepare("read_tag_text", 0);

        c.prepare("update_comment", 2);
        c.prepare("update_post_description", 2);
        c.prepare("update_post_title", 2);
        c.prepare("update_tag_description", 2);
        c.prepare("update_tag_name", 2);
    }

    auto database::create_comment(
        std::string_view post_id,
        std::optional<std::string_view> parent_id,
        std::string_view content
    ) -> comment {
        auto tx = ntx();

        return entix::make_entity<comment>(
            tx,
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
        ntx().exec_prepared("create_object", object_id, preview_id, source_id);
    }

    auto database::create_post(
        std::optional<std::string_view> title,
        std::optional<std::string_view> description,
        const std::vector<std::string>& objects,
        const std::vector<std::string>& tags
    ) -> post_search {
        auto tx = ntx();

        return entix::make_entity<post_search>(
            tx,
            __FUNCTION__,
            title,
            description,
            objects,
            tags
        );
    }

    auto database::create_post_objects(
        std::string_view post_id,
        const std::vector<std::string>& objects,
        unsigned int position
    ) -> post_object_update {
        auto tx = ntx();

        return {
            .objects = entix::make_entities<std::vector<object_preview>>(
                tx,
                __FUNCTION__,
                post_id,
                objects,
                position
            ),
            .date_modified = read_post_date_modified(tx, post_id)
        };
    }

    auto database::create_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, post_id, tag_id);
    }

    auto database::create_site(
        std::string_view scheme,
        std::string_view name,
        std::optional<std::string_view> icon
    ) -> site {
        auto tx = ntx();

        try {
            return entix::make_entity<site>(
                tx,
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

    auto database::create_source(
        std::string_view site_id,
        std::string_view resource
    ) -> source {
        auto tx = ntx();
        return entix::make_entity<source>(
            tx,
            "create_source",
            site_id,
            resource
        );
    }

    auto database::create_tag(std::string_view name) -> std::string {
        return ntx()
            .exec_prepared1("create_tag", name)[0]
            .as<std::string>();
    }

    auto database::create_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        auto tx = ntx();
        return entix::make_entity<tag_name>(
            tx,
            "create_tag_alias",
            tag_id,
            alias
        );
    }

    auto database::create_tag_source(
        std::string_view tag_id,
        std::string_view source_id
    ) -> void {
        ntx().exec_prepared("create_tag_source", tag_id, source_id);
    }

    auto database::delete_post(std::string_view post_id) -> void {
        ntx().exec_prepared("delete_post", post_id);
    }

    auto database::delete_post_objects(
        std::string_view post_id,
        std::span<const range> ranges
    ) -> std::string {
        auto arg = std::vector<std::string>();

        for (const auto& r : ranges) {
            arg.push_back(fmt::format(
                "[{},{}]",
                r.first + 1,
                r.last + 1
            ));
        }

        return ntx().exec_prepared1(
            __FUNCTION__,
            post_id,
            arg
        )[0].as<std::string>();
    }

    auto database::delete_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, post_id, tag_id);
    }

    auto database::delete_tag(std::string_view tag_id) -> void {
        ntx().exec_prepared("delete_tag", tag_id);
    }

    auto database::delete_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        auto tx = ntx();
        return entix::make_entity<tag_name>(
            tx,
            "delete_tag_alias",
            tag_id,
            alias
        );
    }

    auto database::delete_tag_source(
        std::string_view tag_id,
        std::string_view source_id
    ) -> void {
        ntx().exec_prepared("delete_tag_source", tag_id, source_id);
    }

    auto database::move_post_object(
        std::string_view post_id,
        unsigned int old_index,
        unsigned int new_index
    ) -> std::string {
        return ntx().exec_prepared1(
            __FUNCTION__,
            post_id,
            old_index + 1,
            new_index + 1
        )[0].as<std::string>();
    }

    auto database::ntx() -> pqxx::nontransaction {
        return pqxx::nontransaction(connection);
    }

    auto database::prune() -> void {
        ntx().exec_prepared(__FUNCTION__);
    }

    auto database::prune_objects(
        std::function<bool(std::span<const std::string>)>&& on_deleted
    ) -> void {
        auto tx = pqxx::transaction(connection);
        const auto objects =
            entix::make_objects<std::vector<std::string>>(tx, __FUNCTION__);

        if (on_deleted(objects)) tx.commit();
    }

    auto database::read_comments(
        std::string_view post_id
    ) -> std::vector<comment> {
        auto tx = ntx();
        return entix::make_entities<std::vector<comment>>(
            tx,
            "read_comments",
            post_id
        );
    }

    auto database::read_object(std::string_view object_id) -> object {
        auto tx = ntx();
        return entix::make_entity<object>(tx, "read_object", object_id);
    }

    auto database::read_object_posts(
        std::string_view object_id
    ) -> std::vector<post_preview> {
        auto tx = ntx();
        return entix::make_entities<std::vector<post_preview>>(
            tx,
            "read_object_posts",
            object_id
        );
    }

    auto database::read_post(std::string_view post_id) -> post {
        auto tx = ntx();
        return entix::make_entity<post>(tx, "read_post", post_id);
    }

    auto database::read_post_date_modified(
        pqxx::transaction_base& tx,
        std::string_view post_id
    ) -> std::string {
        return tx.exec_prepared1(__FUNCTION__, post_id)[0].as<std::string>();
    }

    auto database::read_posts(
        const std::vector<std::string>& posts
    ) -> std::vector<post_preview> {
        auto tx = ntx();
        return entix::make_entities<std::vector<post_preview>>(
            tx,
            __FUNCTION__,
            posts
        );
    }

    auto database::read_post_objects(
        std::string_view post_id
    ) -> std::vector<object_preview> {
        auto tx = ntx();
        return entix::make_entities<std::vector<object_preview>>(
            tx,
            "read_post_objects",
            post_id
        );
    }

    auto database::read_post_search() -> std::vector<post_search> {
        auto tx = ntx();
        return entix::make_entities<std::vector<post_search>>(tx, __FUNCTION__);
    }

    auto database::read_post_tags(
        std::string_view post_id
    ) -> std::vector<tag_preview> {
        auto tx = ntx();
        return entix::make_entities<std::vector<tag_preview>>(
            tx,
            "read_post_tags",
            post_id
        );
    }

    auto database::read_site(
        std::string_view scheme,
        std::string_view host
    ) -> std::optional<std::string> {
        return ntx()
            .exec_prepared1("read_site", scheme, host)[0]
            .as<std::optional<std::string>>();
    }

    auto database::read_tag(std::string_view tag_id) -> tag {
        auto tx = ntx();

        try {
            return entix::make_entity<tag>(tx, "read_tag", tag_id);
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw minty_error(
                "Tag with ID ({}) does not exist",
                tag_id
            );
        }
    }

    auto database::read_tag_previews(
        const std::vector<std::string>& tags
    ) -> std::vector<tag_preview> {
        auto tx = ntx();
        return entix::make_entities<std::vector<tag_preview>>(
            tx,
            "read_tag_previews",
            tags
        );
    }

    auto database::read_tag_sources(
        std::string_view tag_id
    ) -> std::vector<source> {
        auto tx = ntx();
        return entix::make_entities<std::vector<source>>(
            tx,
            "read_tag_sources",
            tag_id
        );
    }

    auto database::read_tag_text() -> std::vector<tag_text> {
        auto tx = ntx();
        return entix::make_entities<std::vector<tag_text>>(tx, __FUNCTION__);
    }

    auto database::update_comment(
        std::string_view comment_id,
        std::string_view content
    ) -> void {
        ntx().exec_prepared(__FUNCTION__, comment_id, content);
    }

    auto database::update_post_description(
        std::string_view post_id,
        std::string_view description
    ) -> post_update {
        auto tx = ntx();
        return entix::make_entity<post_update>(
            tx,
            __FUNCTION__,
            post_id,
            description
        );
    }

    auto database::update_post_title(
        std::string_view post_id,
        std::string_view title
    ) -> post_update {
        auto tx = ntx();
        return entix::make_entity<post_update>(
            tx,
            __FUNCTION__,
            post_id,
            title
        );
    }

    auto database::update_tag_description(
        std::string_view tag_id,
        std::string_view description
    ) -> std::optional<std::string> {
        return ntx()
            .exec_prepared1("update_tag_description", tag_id, description)[0]
            .as<std::optional<std::string>>();
    }

    auto database::update_tag_name(
        std::string_view tag_id,
        std::string_view name
    ) -> tag_name_update {
        auto tx = ntx();
        return entix::make_entity<tag_name_update>(
            tx,
            "update_tag_name",
            tag_id,
            name
        );
    }
}
