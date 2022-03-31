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

        c.prepare("create_comment", {"integer", "integer", "text"});
        c.prepare("create_object", {"uuid", "uuid", "integer"});
        c.prepare("create_post", {"text", "text", "uuid[]", "integer[]"});
        c.prepare("create_post_objects", {"integer", "uuid[]", "smallint"});
        c.prepare("create_post_tag", {"integer", "integer"});
        c.prepare("create_related_post", {"integer", "integer"});
        c.prepare("create_site", {"text", "text", "uuid"});
        c.prepare("create_source", {"integer", "text"});
        c.prepare("create_tag", {"text"});
        c.prepare("create_tag_alias", {"integer", "text"});
        c.prepare("create_tag_source", {"integer", "integer"});
        c.prepare("delete_post", {"integer"});
        c.prepare("delete_post_objects", {"integer", "uuid[]"});
        c.prepare("delete_post_objects_ranges", {"integer", "int4range[]"});
        c.prepare("delete_post_tag", {"integer", "integer"});
        c.prepare("delete_related_post", {"integer", "integer"});
        c.prepare("delete_tag", {"integer"});
        c.prepare("delete_tag_alias", {"integer", "text"});
        c.prepare("delete_tag_source", {"integer", "integer"});
        c.prepare("move_post_object", {"integer", "integer", "integer"});
        c.prepare("move_post_objects", {"integer", "uuid[]", "uuid"});
        c.prepare("prune", {});
        c.prepare("prune_objects", {});
        c.prepare("read_comments", {"integer"});
        c.prepare("read_object", {"uuid"});
        c.prepare("read_object_posts", {"uuid"});
        c.prepare("read_post", {"integer"});
        c.prepare("read_posts", {"integer[]"});
        c.prepare("read_post_objects", {"integer"});
        c.prepare("read_post_search", {});
        c.prepare("read_post_tags", {"integer"});
        c.prepare("read_related_posts", {"integer"});
        c.prepare("read_site", {"text", "text"});
        c.prepare("read_tag", {"integer"});
        c.prepare("read_tag_previews", {"integer[]"});
        c.prepare("read_tag_sources", {"integer"});
        c.prepare("read_tag_text", {});
        c.prepare("update_comment", {"integer", "text"});
        c.prepare("update_object_preview", {"uuid", "uuid"});
        c.prepare("update_post_description", {"integer", "text"});
        c.prepare("update_post_title", {"integer", "text"});
        c.prepare("update_tag_description", {"integer", "text"});
        c.prepare("update_tag_name", {"integer", "text"});
    }

    auto database::ntx() -> pqxx::nontransaction {
        return pqxx::nontransaction(*connection);
    }
}
