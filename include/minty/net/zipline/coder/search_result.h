#pragma once

#include <minty/core/model.h>

#include <zipline/zipline>

namespace zipline {
    template <typename T, io::reader Reader>
    ZIPLINE_OBJECT_DECODER_TEMPLATE(
        minty::core::search_result<T>,
        &minty::core::search_result<T>::total,
        &minty::core::search_result<T>::hits
    );

    template <typename T, io::writer Writer>
    ZIPLINE_OBJECT_ENCODER_TEMPLATE(
        minty::core::search_result<T>,
        &minty::core::search_result<T>::total,
        &minty::core::search_result<T>::hits
    );
}
