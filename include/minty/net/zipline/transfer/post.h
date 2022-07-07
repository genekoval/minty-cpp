#pragma once

#include <minty/core/model.h>

#include <zipline/zipline>

namespace zipline {
    template <typename Socket, typename T>
    ZIPLINE_OBJECT_TEMPLATE(
        minty::core::modification<T>,
        &minty::core::modification<T>::date_modified,
        &minty::core::modification<T>::new_value
    );

    ZIPLINE_OBJECT(
        minty::core::post,
        &minty::core::post::id,
        &minty::core::post::title,
        &minty::core::post::description,
        &minty::core::post::date_created,
        &minty::core::post::date_modified,
        &minty::core::post::objects,
        &minty::core::post::posts,
        &minty::core::post::tags
    );

    ZIPLINE_OBJECT(
        minty::core::post_parts,
        &minty::core::post_parts::title,
        &minty::core::post_parts::description,
        &minty::core::post_parts::objects,
        &minty::core::post_parts::tags
    );

    ZIPLINE_OBJECT(
        minty::core::post_preview,
        &minty::core::post_preview::id,
        &minty::core::post_preview::title,
        &minty::core::post_preview::preview,
        &minty::core::post_preview::comment_count,
        &minty::core::post_preview::object_count,
        &minty::core::post_preview::date_created
    );

    ZIPLINE_OBJECT(
        minty::core::post_query,
        &minty::core::post_query::from,
        &minty::core::post_query::size,
        &minty::core::post_query::text,
        &minty::core::post_query::tags,
        &minty::core::post_query::sort
    );

    ZIPLINE_OBJECT(
        minty::core::post_query::sort_type,
        &minty::core::post_query::sort_type::order,
        &minty::core::post_query::sort_type::value
    );

    ZIPLINE_OBJECT(
        minty::core::range,
        &minty::core::range::first,
        &minty::core::range::last
    );
}
