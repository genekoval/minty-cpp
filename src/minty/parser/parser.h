#pragma once

#include <minty/minty>
#include <commline/commline>

namespace commline {
    template <>
    auto parse(std::string_view argument) -> minty::core::post_query::sort_type;
}
