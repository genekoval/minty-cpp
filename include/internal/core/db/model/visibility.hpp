#pragma once

#include <minty/model/visibility.hpp>

#include <pg++/pg++>

namespace pg {
    template <>
    struct enum_type<minty::visibility> {
        static auto from_string(std::string_view string) -> minty::visibility;

        static auto to_string(minty::visibility visibility) -> std::string_view;
    };
}

PG_ENUM(minty::visibility, "visibility");
