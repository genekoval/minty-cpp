#include "entity.h"

#include <minty/error.h>
#include <minty/repo/db/database.h>
#include <minty/repo/db/db.h>

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

namespace minty::repo::db {
    database::database(
        std::string_view connection_string,
        int connection_count
    ) :
        connections(connection_string, connection_count, prepare_connection)
    {}
}
