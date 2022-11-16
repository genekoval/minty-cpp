#pragma once

#include <minty/core/model.h>

#include <zipline/zipline>

namespace zipline {
    template <typename Socket, typename T>
    ZIPLINE_OBJECT_TEMPLATE(
        minty::core::search_result<T>,
        &minty::core::search_result<T>::total,
        &minty::core::search_result<T>::hits
    );
}
