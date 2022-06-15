#pragma once

#include <minty/repo/db/model.h>

#include <entix/entix>

namespace entix {
    ENTITY(
        minty::repo::db::comment,
        &minty::repo::db::comment::id,
        &minty::repo::db::comment::post_id,
        &minty::repo::db::comment::parent_id,
        &minty::repo::db::comment::indent,
        &minty::repo::db::comment::content,
        &minty::repo::db::comment::date_created
    );

    ENTITY(
        minty::repo::db::object,
        &minty::repo::db::object::id,
        &minty::repo::db::object::preview_id,
        &minty::repo::db::object::src
    );

    ENTITY(
        minty::repo::db::object_error,
        &minty::repo::db::object_error::id,
        &minty::repo::db::object_error::message
    );

    ENTITY(
        minty::repo::db::object_preview,
        &minty::repo::db::object_preview::id,
        &minty::repo::db::object_preview::preview_id
    );

    ENTITY(
        minty::repo::db::post,
        &minty::repo::db::post::id,
        &minty::repo::db::post::title,
        &minty::repo::db::post::description,
        &minty::repo::db::post::date_created,
        &minty::repo::db::post::date_modified
    );

    ENTITY(
        minty::repo::db::post_preview,
        &minty::repo::db::post_preview::id,
        &minty::repo::db::post_preview::title,
        &minty::repo::db::post_preview::preview,
        &minty::repo::db::post_preview::comment_count,
        &minty::repo::db::post_preview::object_count,
        &minty::repo::db::post_preview::date_created
    );

    ENTITY(
        minty::repo::db::post_search,
        &minty::repo::db::post_search::id,
        &minty::repo::db::post_search::title,
        &minty::repo::db::post_search::description,
        &minty::repo::db::post_search::date_created,
        &minty::repo::db::post_search::date_modified,
        &minty::repo::db::post_search::tags
    );

    ENTITY(
        minty::repo::db::post_update,
        &minty::repo::db::post_update::id,
        &minty::repo::db::post_update::new_data,
        &minty::repo::db::post_update::date_modified
    );

    ENTITY(
        minty::repo::db::site,
        &minty::repo::db::site::id,
        &minty::repo::db::site::scheme,
        &minty::repo::db::site::host,
        &minty::repo::db::site::icon
    );

    ENTITY(
        minty::repo::db::source,
        &minty::repo::db::source::id,
        &minty::repo::db::source::resource,
        &minty::repo::db::source::website
    );

    ENTITY(
        minty::repo::db::tag,
        &minty::repo::db::tag::id,
        &minty::repo::db::tag::name,
        &minty::repo::db::tag::aliases,
        &minty::repo::db::tag::description,
        &minty::repo::db::tag::avatar,
        &minty::repo::db::tag::banner,
        &minty::repo::db::tag::post_count,
        &minty::repo::db::tag::date_created
    );

    ENTITY(
        minty::repo::db::tag_name,
        &minty::repo::db::tag_name::name,
        &minty::repo::db::tag_name::aliases
    );

    ENTITY(
        minty::repo::db::tag_name_update,
        &minty::repo::db::tag_name_update::names,
        &minty::repo::db::tag_name_update::old_name
    );

    ENTITY(
        minty::repo::db::tag_preview,
        &minty::repo::db::tag_preview::id,
        &minty::repo::db::tag_preview::name,
        &minty::repo::db::tag_preview::avatar
    );

    ENTITY(
        minty::repo::db::tag_text,
        &minty::repo::db::tag_text::id,
        &minty::repo::db::tag_text::names
    );
}
