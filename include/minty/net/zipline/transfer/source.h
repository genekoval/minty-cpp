#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::core::site> {
        using type = minty::core::site;

        using id_t = decltype(type::id);
        using name_t = decltype(type::name);
        using homepage_t = decltype(type::homepage);
        using thumbnail_id_t = decltype(type::thumbnail_id);

        static auto read(const minty::net::socket&) -> type;
        static auto write(const minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::source> {
        using type = minty::core::source;

        using id_t = decltype(type::id);
        using url_t = decltype(type::url);
        using website_t = decltype(type::website);

        static auto read(const minty::net::socket&) -> type;
        static auto write(const minty::net::socket&, const type&) -> void;
    };
}
