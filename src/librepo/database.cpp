#include "entity/entity.h"

#include <minty/error.h>
#include <minty/repo/db/database.h>
#include <minty/repo/db/db.h>

#include <fmt/format.h>

namespace minty::repo::db {
    database::database(std::string_view connection_string) :
        connection(pqxx::connection(std::string(connection_string)))
    {
        auto c = entix::connection(*connection);

        c.prepare("create_comment", {"uuid", "text"});
        c.prepare("create_object", {"uuid", "uuid", "bigint"});
        c.prepare("create_post", {"text", "text", "uuid[]", "uuid[]"});
        c.prepare("create_post_objects", {"uuid", "uuid[]", "smallint"});
        c.prepare("create_post_tag", {"uuid", "uuid"});
        c.prepare("create_related_post", {"uuid", "uuid"});
        c.prepare("create_reply", {"uuid", "text"});
        c.prepare("create_site", {"text", "text", "uuid"});
        c.prepare("create_source", {"bigint", "text"});
        c.prepare("create_tag", {"text"});
        c.prepare("create_tag_alias", {"uuid", "text"});
        c.prepare("create_tag_source", {"uuid", "bigint"});
        c.prepare("delete_post", {"uuid"});
        c.prepare("delete_post_objects", {"uuid", "uuid[]"});
        c.prepare("delete_post_objects_ranges", {"uuid", "int4range[]"});
        c.prepare("delete_post_tag", {"uuid", "uuid"});
        c.prepare("delete_related_post", {"uuid", "uuid"});
        c.prepare("delete_tag", {"uuid"});
        c.prepare("delete_tag_alias", {"uuid", "text"});
        c.prepare("delete_tag_source", {"uuid", "bigint"});
        c.prepare("move_post_object", {"uuid", "integer", "integer"});
        c.prepare("move_post_objects", {"uuid", "uuid[]", "uuid"});
        c.prepare("prune", {});
        c.prepare("prune_objects", {});
        c.prepare("read_comments", {"uuid"});
        c.prepare("read_object", {"uuid"});
        c.prepare("read_object_posts", {"uuid"});
        c.prepare("read_post", {"uuid"});
        c.prepare("read_posts", {"uuid[]"});
        c.prepare("read_post_objects", {"uuid"});
        c.prepare("read_post_search", {});
        c.prepare("read_post_tags", {"uuid"});
        c.prepare("read_related_posts", {"uuid"});
        c.prepare("read_site", {"text", "text"});
        c.prepare("read_tag", {"uuid"});
        c.prepare("read_tag_previews", {"uuid[]"});
        c.prepare("read_tag_sources", {"uuid"});
        c.prepare("read_tag_text", {});
        c.prepare("update_comment", {"uuid", "text"});
        c.prepare("update_object_preview", {"uuid", "uuid"});
        c.prepare("update_post_description", {"uuid", "text"});
        c.prepare("update_post_title", {"uuid", "text"});
        c.prepare("update_tag_description", {"uuid", "text"});
        c.prepare("update_tag_name", {"uuid", "text"});
    }

    auto database::ntx() -> pqxx::nontransaction {
        return pqxx::nontransaction(*connection);
    }
}
