#include <minty/model/data_size.hpp>

#include <ext/data_size.h>

namespace minty {
    constexpr auto decimal_places = 2u;

    data_size::data_size(uintmax_t bytes) :
        bytes(bytes),
        formatted(ext::data_size::format(bytes).str(decimal_places))
    {}
}
