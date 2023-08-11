#pragma once

#include <minty/model/post_query.hpp>
#include <minty/model/sort_order.hpp>
#include <minty/model/visibility.hpp>

#include <ext/string.h>
#include <http/http>

namespace http {
    template <>
    struct parser<minty::post_sort_value> {
        static auto parse(std::string_view string) -> minty::post_sort_value;
    };

    template <>
    struct parser<minty::sort_order> {
        static auto parse(std::string_view string) -> minty::sort_order;
    };

    template <>
    struct parser<minty::visibility> {
        static auto parse(std::string_view string) -> minty::visibility;
    };
}
