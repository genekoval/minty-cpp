#pragma once

#include <minty/core/model.h>

#include <zipline/zipline>

namespace zipline {
    ZIPLINE_OBJECT(
        minty::core::source,
        &minty::core::source::id,
        &minty::core::source::url,
        &minty::core::source::icon
    );
}
