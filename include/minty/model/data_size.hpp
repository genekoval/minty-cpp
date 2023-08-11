#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <zipline/zipline>

namespace minty {
    struct data_size {
        uintmax_t bytes;
        std::string formatted;

        data_size() = default;
        data_size(uintmax_t bytes);
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        data_size,
        bytes,
        formatted
    );
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::data_size,
        &minty::data_size::bytes,
        &minty::data_size::formatted
    );
}
