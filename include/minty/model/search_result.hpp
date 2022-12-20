#pragma once

#include <zipline/zipline>

namespace minty {
    template <typename T>
    struct search_result {
        std::uint32_t total;
        std::vector<T> hits;
    };
}

namespace zipline {
    template <typename T, io::reader Reader>
    ZIPLINE_OBJECT_DECODER_TEMPLATE(
        minty::search_result<T>,
        &minty::search_result<T>::total,
        &minty::search_result<T>::hits
    );

    template <typename T, io::writer Writer>
    ZIPLINE_OBJECT_ENCODER_TEMPLATE(
        minty::search_result<T>,
        &minty::search_result<T>::total,
        &minty::search_result<T>::hits
    );
}
