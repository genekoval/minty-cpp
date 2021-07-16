#pragma once

#include <minty/core/model.h>

#include <zipline/zipline>

namespace zipline {
    ZIPLINE_OBJECT(
        minty::core::object,
        &minty::core::object::id,
        &minty::core::object::hash,
        &minty::core::object::size,
        &minty::core::object::mime_type,
        &minty::core::object::date_added,
        &minty::core::object::preview_id,
        &minty::core::object::src,
        &minty::core::object::posts
    );

    ZIPLINE_OBJECT(
        minty::core::object_preview,
        &minty::core::object_preview::id,
        &minty::core::object_preview::preview_id,
        &minty::core::object_preview::mime_type
    );

    ZIPLINE_OBJECT(
        minty::core::data_size,
        &minty::core::data_size::bytes,
        &minty::core::data_size::formatted
    );
}
