#include <minty/repo/db/entity.h>

namespace minty::repo::db {
    READ_ENTITY(
        comment,
        &comment::id,
        &comment::post_id,
        &comment::parent_id,
        &comment::indent,
        &comment::content,
        &comment::date_created
    );

    READ_ENTITY(
        object,
        &object::id,
        &object::preview_id,
        &object::src
    );

    READ_ENTITY(
        object_preview,
        &object_preview::id,
        &object_preview::preview_id
    );

    READ_ENTITY(
        post,
        &post::id,
        &post::title,
        &post::description,
        &post::date_created,
        &post::date_modified
    );

    READ_ENTITY(
        post_preview,
        &post_preview::id,
        &post_preview::title,
        &post_preview::preview_id,
        &post_preview::comment_count,
        &post_preview::object_count,
        &post_preview::date_created
    );

    READ_ENTITY(
        post_search,
        &post_search::id,
        &post_search::title,
        &post_search::description,
        &post_search::date_created,
        &post_search::date_modified,
        &post_search::tags
    );

    READ_ENTITY(
        site,
        &site::id,
        &site::scheme,
        &site::host,
        &site::icon
    );

    READ_ENTITY(
        source,
        &source::id,
        &source::resource,
        &source::website
    );

    READ_ENTITY(
        tag,
        &tag::id,
        &tag::name,
        &tag::aliases,
        &tag::description,
        &tag::avatar,
        &tag::banner,
        &tag::post_count,
        &tag::date_created
    );

    READ_ENTITY(
        tag_name,
        &tag_name::name,
        &tag_name::aliases
    );

    READ_ENTITY(
        tag_name_update,
        &tag_name_update::names,
        &tag_name_update::old_name
    );

    READ_ENTITY(
        tag_preview,
        &tag_preview::id,
        &tag_preview::name,
        &tag_preview::avatar
    );

    READ_ENTITY(
        tag_text,
        &tag_text::id,
        &tag_text::names
    );
}
