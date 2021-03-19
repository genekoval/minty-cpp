#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::core::tag> {
        using type = minty::core::tag;

        using id_t = decltype(type::id);
        using name_t = decltype(type::name);
        using color_t = decltype(type::color);
        using date_created_t = decltype(type::date_created);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };
}
