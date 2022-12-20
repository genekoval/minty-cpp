#pragma once

#include "time_point.hpp"

#include <zipline/zipline>

namespace minty {
    template <typename T>
    struct modification {
        time_point date_modified;
        T new_value;
    };
}

namespace zipline {
    template <typename T, io::reader Reader>
    ZIPLINE_OBJECT_DECODER_TEMPLATE(
        minty::modification<T>,
        &minty::modification<T>::date_modified,
        &minty::modification<T>::new_value
    );

    template <typename T, io::writer Writer>
    ZIPLINE_OBJECT_ENCODER_TEMPLATE(
        minty::modification<T>,
        &minty::modification<T>::date_modified,
        &minty::modification<T>::new_value
    );
}
