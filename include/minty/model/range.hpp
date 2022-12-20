#pragma once

#include <zipline/zipline>

namespace minty {
    struct range {
        using index_type = std::int32_t;

        index_type first;
        index_type last;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::range,
        &minty::range::first,
        &minty::range::last
    );
}
