#pragma once

#include <minty/core/model.h>

#include <zipline/zipline>

namespace zipline {
    ZIPLINE_OBJECT(
        minty::core::post,
        &minty::core::post::id,
        &minty::core::post::title,
        &minty::core::post::description,
        &minty::core::post::date_created,
        &minty::core::post::date_modified,
        &minty::core::post::objects,
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
        &minty::core::post_preview::preview_id,
        &minty::core::post_preview::comment_count,
        &minty::core::post_preview::object_count,
        &minty::core::post_preview::date_created
    );

    ZIPLINE_OBJECT(
        minty::core::post_search,
        &minty::core::post_search::id,
        &minty::core::post_search::title,
        &minty::core::post_search::description,
        &minty::core::post_search::date_created,
        &minty::core::post_search::date_modified,
        &minty::core::post_search::tags
    );

    ZIPLINE_OBJECT(
        minty::core::range,
        &minty::core::range::first,
        &minty::core::range::last
    );
}
