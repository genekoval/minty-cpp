#pragma once

#include <internal/core/db/model/object.hpp>
#include <internal/core/db/model/object_preview.hpp>
#include <internal/core/db/model/post.hpp>
#include <internal/core/db/model/post_preview.hpp>
#include <internal/core/db/model/post_search.hpp>
#include <internal/core/db/model/post_update.hpp>
#include <internal/core/db/model/site.hpp>
#include <internal/core/db/model/source.hpp>
#include <internal/core/db/model/tag.hpp>
#include <internal/core/db/model/tag_search.hpp>
#include <internal/core/db/model/tag_name_update.hpp>
#include <minty/model/comment.hpp>
#include <minty/model/object_error.hpp>
#include <minty/model/tag_name.hpp>
#include <minty/model/tag_preview.hpp>

#include <entix/entix>

namespace entix {
    ENTITY(
        minty::comment,
        &minty::comment::id,
        &minty::comment::post_id,
        &minty::comment::parent_id,
        &minty::comment::indent,
        &minty::comment::content,
        &minty::comment::date_created
    );

    ENTITY(
        minty::core::db::object,
        &minty::core::db::object::id,
        &minty::core::db::object::preview_id,
        &minty::core::db::object::src
    );

    ENTITY(
        minty::object_error,
        &minty::object_error::id,
        &minty::object_error::message
    );

    ENTITY(
        minty::core::db::object_preview,
        &minty::core::db::object_preview::id,
        &minty::core::db::object_preview::preview_id
    );

    ENTITY(
        minty::core::db::post,
        &minty::core::db::post::id,
        &minty::core::db::post::title,
        &minty::core::db::post::description,
        &minty::core::db::post::date_created,
        &minty::core::db::post::date_modified
    );

    ENTITY(
        minty::core::db::post_preview,
        &minty::core::db::post_preview::id,
        &minty::core::db::post_preview::title,
        &minty::core::db::post_preview::preview,
        &minty::core::db::post_preview::comment_count,
        &minty::core::db::post_preview::object_count,
        &minty::core::db::post_preview::date_created
    );

    ENTITY(
        minty::core::db::post_search,
        &minty::core::db::post_search::id,
        &minty::core::db::post_search::title,
        &minty::core::db::post_search::description,
        &minty::core::db::post_search::created,
        &minty::core::db::post_search::modified,
        &minty::core::db::post_search::tags
    );

    ENTITY(
        minty::core::db::post_update,
        &minty::core::db::post_update::id,
        &minty::core::db::post_update::new_data,
        &minty::core::db::post_update::date_modified
    );

    ENTITY(
        minty::core::db::site,
        &minty::core::db::site::id,
        &minty::core::db::site::scheme,
        &minty::core::db::site::host,
        &minty::core::db::site::icon
    );

    ENTITY(
        minty::core::db::source,
        &minty::core::db::source::id,
        &minty::core::db::source::resource,
        &minty::core::db::source::website
    );

    ENTITY(
        minty::core::db::tag,
        &minty::core::db::tag::id,
        &minty::core::db::tag::name,
        &minty::core::db::tag::aliases,
        &minty::core::db::tag::description,
        &minty::core::db::tag::avatar,
        &minty::core::db::tag::banner,
        &minty::core::db::tag::post_count,
        &minty::core::db::tag::date_created
    );

    ENTITY(
        minty::tag_name,
        &minty::tag_name::name,
        &minty::tag_name::aliases
    );

    ENTITY(
        minty::core::db::tag_name_update,
        &minty::core::db::tag_name_update::names,
        &minty::core::db::tag_name_update::old_name
    );

    ENTITY(
        minty::tag_preview,
        &minty::tag_preview::id,
        &minty::tag_preview::name,
        &minty::tag_preview::avatar
    );

    ENTITY(
        minty::core::db::tag_search,
        &minty::core::db::tag_search::id,
        &minty::core::db::tag_search::names
    );
}
