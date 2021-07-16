
#pragma once

#include <minty/core/model.h>

#include <zipline/zipline>

namespace zipline {
    ZIPLINE_OBJECT(
        minty::core::tag,
        &minty::core::tag::id,
        &minty::core::tag::name,
        &minty::core::tag::aliases,
        &minty::core::tag::description,
        &minty::core::tag::avatar,
        &minty::core::tag::banner,
        &minty::core::tag::sources,
        &minty::core::tag::post_count,
        &minty::core::tag::date_created
    );

    ZIPLINE_OBJECT(
        minty::core::tag_name,
        &minty::core::tag_name::name,
        &minty::core::tag_name::aliases
    );

    ZIPLINE_OBJECT(
        minty::core::tag_preview,
        &minty::core::tag_preview::id,
        &minty::core::tag_preview::name,
        &minty::core::tag_preview::avatar
    );

    ZIPLINE_OBJECT(
        minty::core::tag_text,
        &minty::core::tag_text::id,
        &minty::core::tag_text::names
    );
}
