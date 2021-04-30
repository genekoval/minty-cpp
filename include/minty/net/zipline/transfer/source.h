#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::core::source> {
        using type = minty::core::source;

        using id_t = decltype(type::id);
        using url_t = decltype(type::url);
        using icon_t = decltype(type::icon);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };
}
