#include "db.hpp"
#include "entity.hpp"

#include <internal/core/db/database.hpp>
#include <minty/error.h>

#include <fmt/format.h>

using entix::prepare;

namespace {
    auto prepare_connection(pqxx::connection& c) -> void {
        prepare(c, "create_comment", {"uuid", "text"});
        prepare(c, "create_object", {"uuid", "uuid", "bigint"});
        prepare(c, "create_object_preview_error", {"uuid", "text"});
        prepare(c, "create_post", {"text", "text", "uuid[]", "uuid[]"});
        prepare(c, "create_post_objects", {"uuid", "uuid[]", "smallint"});
        prepare(c, "create_post_tag", {"uuid", "uuid"});
        prepare(c, "create_related_post", {"uuid", "uuid"});
        prepare(c, "create_reply", {"uuid", "text"});
        prepare(c, "create_site", {"text", "text", "uuid"});
        prepare(c, "create_source", {"bigint", "text"});
        prepare(c, "create_tag", {"text"});
        prepare(c, "create_tag_alias", {"uuid", "text"});
        prepare(c, "create_tag_source", {"uuid", "bigint"});
        prepare(c, "delete_object_preview_error", {"uuid"});
        prepare(c, "delete_post", {"uuid"});
        prepare(c, "delete_post_objects", {"uuid", "uuid[]"});
        prepare(c, "delete_post_objects_ranges", {"uuid", "int4range[]"});
        prepare(c, "delete_post_tag", {"uuid", "uuid"});
        prepare(c, "delete_related_post", {"uuid", "uuid"});
        prepare(c, "delete_tag", {"uuid"});
        prepare(c, "delete_tag_alias", {"uuid", "text"});
        prepare(c, "delete_tag_source", {"uuid", "bigint"});
        prepare(c, "move_post_object", {"uuid", "integer", "integer"});
        prepare(c, "move_post_objects", {"uuid", "uuid[]", "uuid"});
        prepare(c, "prune", {});
        prepare(c, "prune_objects", {});
        prepare(c, "read_comment", {"uuid"});
        prepare(c, "read_comments", {"uuid"});
        prepare(c, "read_object", {"uuid"});
        prepare(c, "read_object_preview_errors", {});
        prepare(c, "read_object_posts", {"uuid"});
        prepare(c, "read_post", {"uuid"});
        prepare(c, "read_posts", {"uuid[]"});
        prepare(c, "read_post_objects", {"uuid"});
        prepare(c, "read_post_search", {});
        prepare(c, "read_post_tags", {"uuid"});
        prepare(c, "read_related_posts", {"uuid"});
        prepare(c, "read_site", {"text", "text"});
        prepare(c, "read_tag", {"uuid"});
        prepare(c, "read_tag_previews", {"uuid[]"});
        prepare(c, "read_tag_sources", {"uuid"});
        prepare(c, "read_tag_text", {});
        prepare(c, "read_total_objects", {});
        prepare(c, "update_comment", {"uuid", "text"});
        prepare(c, "update_object_preview", {"uuid", "uuid"});
        prepare(c, "update_post_description", {"uuid", "text"});
        prepare(c, "update_post_title", {"uuid", "text"});
        prepare(c, "update_tag_description", {"uuid", "text"});
        prepare(c, "update_tag_name", {"uuid", "text"});
    }
}

namespace minty::core::db {
    database::database(
        std::string_view connection_string,
        int connection_count
    ) :
        connections(connection_string, connection_count, prepare_connection)
    {}

    auto database::create_comment(
        const UUID::uuid& post_id,
        std::string_view content
    ) -> comment {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return entix::make_entity<comment>(
            tx,
            __FUNCTION__,
            post_id,
            content
        );
    }

    auto database::create_object(
        const UUID::uuid& object_id,
        const std::optional<UUID::uuid>& preview_id,
        const std::optional<std::string_view>& source_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, object_id, preview_id, source_id);
    }

    auto database::create_object_preview_error(
        const UUID::uuid& object_id,
        std::string_view message
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, object_id, message);
    }

    auto database::create_post(
        std::string_view title,
        std::string_view description,
        const std::vector<UUID::uuid>& objects,
        const std::vector<UUID::uuid>& tags
    ) -> post_search {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

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
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        std::int16_t position
    ) -> decltype(post::date_modified) {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx
            .exec_prepared1(__FUNCTION__, post_id, objects, position)[0]
            .as<decltype(post::date_modified)>();
    }

    auto database::create_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, post_id, tag_id);
    }

    auto database::create_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, post_id, related);
    }

    auto database::create_reply(
        const UUID::uuid& parent_id,
        std::string_view content
    ) -> comment {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return entix::make_entity<comment>(
            tx,
            __FUNCTION__,
            parent_id,
            content
        );
    }

    auto database::create_site(
        std::string_view scheme,
        std::string_view name,
        std::optional<UUID::uuid> icon
    ) -> site {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        try {
            return entix::make_entity<site>(
                tx,
                __FUNCTION__,
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
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<source>(
            tx,
            __FUNCTION__,
            site_id,
            resource
        );
    }

    auto database::create_tag(std::string_view name) -> UUID::uuid {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx
            .exec_prepared1(__FUNCTION__, name)[0]
            .as<UUID::uuid>();
    }

    auto database::create_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> tag_name {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<tag_name>(
            tx,
            __FUNCTION__,
            tag_id,
            alias
        );
    }

    auto database::create_tag_source(
        const UUID::uuid& tag_id,
        std::string_view source_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, tag_id, source_id);
    }


    auto database::delete_object_preview_error(
        const UUID::uuid& object_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, object_id);
    }

    auto database::delete_post(const UUID::uuid& post_id) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, post_id);
    }

    auto database::delete_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects
    ) -> decltype(post::date_modified) {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(
            __FUNCTION__,
            post_id,
            objects
        )[0].as<decltype(post::date_modified)>();
    }

    auto database::delete_post_objects_ranges(
        const UUID::uuid& post_id,
        std::span<const range> ranges
    ) -> decltype(post::date_modified) {
        auto arg = std::vector<std::string>();

        for (const auto& r : ranges) {
            arg.push_back(fmt::format(
                "[{},{}]",
                r.first + 1,
                r.last + 1
            ));
        }

        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(
            __FUNCTION__,
            post_id,
            arg
        )[0].as<decltype(post::date_modified)>();
    }

    auto database::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, post_id, tag_id);
    }

    auto database::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, post_id, related);
    }

    auto database::delete_tag(const UUID::uuid& tag_id) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, tag_id);
    }

    auto database::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> tag_name {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<tag_name>(
            tx,
            __FUNCTION__,
            tag_id,
            alias
        );
    }

    auto database::delete_tag_source(
        const UUID::uuid& tag_id,
        std::string_view source_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, tag_id, source_id);
    }

    auto database::move_post_object(
        const UUID::uuid& post_id,
        unsigned int old_index,
        unsigned int new_index
    ) -> decltype(post::date_modified) {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(
            __FUNCTION__,
            post_id,
            old_index + 1,
            new_index + 1
        )[0].as<decltype(post::date_modified)>();
    }

    auto database::move_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        const std::optional<UUID::uuid>& destination
    ) -> decltype(post::date_modified) {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(
            __FUNCTION__,
            post_id,
            objects,
            destination
        )[0].as<decltype(post::date_modified)>();
    }

    auto database::prune() -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__);
    }

    auto database::prune_objects(
        const std::function<ext::task<bool>(
            std::span<const UUID::uuid>
        )>& on_deleted
    ) -> ext::task<> {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        const auto objects =
            entix::make_objects<std::vector<UUID::uuid>>(tx, __FUNCTION__);

        if (co_await on_deleted(objects)) tx.commit();
    }

    auto database::read_comment(const UUID::uuid& comment_id) -> comment {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return entix::make_entity<comment>(tx, __FUNCTION__, comment_id);
    }

    auto database::read_comments(
        const UUID::uuid& post_id
    ) -> std::vector<comment> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<comment>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }

    auto database::read_object(const UUID::uuid& object_id) -> object {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<object>(tx, __FUNCTION__, object_id);
    }

    auto database::read_object_posts(
        const UUID::uuid& object_id
    ) -> std::vector<post_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<post_preview>>(
            tx,
            __FUNCTION__,
            object_id
        );
    }

    auto database::read_object_preview_errors() -> std::vector<object_error> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return entix::make_entities<std::vector<object_error>>(
            tx,
            __FUNCTION__
        );
    }

    auto database::read_objects(
        int batch_size
    ) -> ext::generator<std::span<object_preview>> {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        auto stream =
            entix::stream<object_preview>(tx, __FUNCTION__, batch_size);

        while (stream) co_yield stream();

        tx.commit();
    }

    auto database::read_post(const UUID::uuid& post_id) -> post {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<post>(tx, __FUNCTION__, post_id);
    }

    auto database::read_post_objects(
        const UUID::uuid& post_id
    ) -> std::vector<object_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<object_preview>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }

    auto database::read_post_search(
        int batch_size
    ) -> ext::generator<std::span<post_search>> {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        auto stream = entix::stream<post_search>(tx, __FUNCTION__, batch_size);

        while (stream) co_yield stream();

        tx.commit();
    }

    auto database::read_post_tags(
        const UUID::uuid& post_id
    ) -> std::vector<tag_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<tag_preview>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }

    auto database::read_posts(
        const std::vector<UUID::uuid>& posts
    ) -> std::vector<post_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<post_preview>>(
            tx,
            __FUNCTION__,
            posts
        );
    }

    auto database::read_related_posts(
        const UUID::uuid& post_id
    ) -> std::vector<post_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<post_preview>>(
            tx,
            __FUNCTION__,
            post_id
        );
    }

    auto database::read_site(
        std::string_view scheme,
        std::string_view host
    ) -> std::optional<std::string> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx
            .exec_prepared1(__FUNCTION__, scheme, host)[0]
            .as<std::optional<std::string>>();
    }

    auto database::read_tag(const UUID::uuid& tag_id) -> tag {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        try {
            return entix::make_entity<tag>(tx, __FUNCTION__, tag_id);
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw minty_error(
                "Tag with ID ({}) does not exist",
                tag_id
            );
        }
    }

    auto database::read_tag_previews(
        const std::vector<UUID::uuid>& tags
    ) -> std::vector<tag_preview> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<tag_preview>>(
            tx,
            __FUNCTION__,
            tags
        );
    }

    auto database::read_tag_sources(
        const UUID::uuid& tag_id
    ) -> std::vector<source> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entities<std::vector<source>>(
            tx,
            __FUNCTION__,
            tag_id
        );
    }

    auto database::read_tag_text(
        int batch_size
    ) -> ext::generator<std::span<tag_search>> {
        auto connection = connections.connection();
        auto tx = pqxx::work(connection);

        auto stream = entix::stream<tag_search>(tx, __FUNCTION__, batch_size);

        while (stream) co_yield stream();

        tx.commit();
    }

    auto database::read_total_objects() -> std::size_t {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx.exec_prepared1(__FUNCTION__)[0].as<std::size_t>();
    }

    auto database::update_comment(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, comment_id, content);
    }

    auto database::update_object_preview(
        const UUID::uuid& object_id,
        const std::optional<UUID::uuid>& preview_id
    ) -> void {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        tx.exec_prepared(__FUNCTION__, object_id, preview_id);
    }

    auto database::update_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> post_update {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<post_update>(
            tx,
            __FUNCTION__,
            post_id,
            description
        );
    }

    auto database::update_post_title(
        const UUID::uuid& post_id,
        std::string_view title
    ) -> post_update {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<post_update>(
            tx,
            __FUNCTION__,
            post_id,
            title
        );
    }

    auto database::update_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> std::optional<std::string> {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);

        return tx
            .exec_prepared1(__FUNCTION__, tag_id, description)[0]
            .as<std::optional<std::string>>();
    }

    auto database::update_tag_name(
        const UUID::uuid& tag_id,
        std::string_view name
    ) -> tag_name_update {
        auto connection = connections.connection();
        auto tx = pqxx::nontransaction(connection);
        return entix::make_entity<tag_name_update>(
            tx,
            __FUNCTION__,
            tag_id,
            name
        );
    }
}
