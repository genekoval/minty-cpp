#pragma once

#include <ext/string.h>
#include <fmt/format.h>
#include <stdexcept>
#include <zipline/zipline>

namespace minty {
    struct minty_error : zipline::zipline_error {
        using zipline_error::zipline_error;
    };

    struct invalid_data : minty_error {
        using minty_error::minty_error;
    };

    struct not_found : minty_error {
        using minty_error::minty_error;
    };

    using error_list = zipline::error_list<
        invalid_data,
        not_found
    >;
}
