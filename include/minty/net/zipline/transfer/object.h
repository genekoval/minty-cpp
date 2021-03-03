#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::core::object> {
        using type = minty::core::object;

        using id_t = decltype(type::id);
        using hash_t = decltype(type::hash);
        using size_t = decltype(type::size);
        using mime_type_t = decltype(type::mime_type);
        using date_added_t = decltype(type::date_added);
        using preview_id_t = decltype(type::preview_id);
        using src_t = decltype(type::src);

        static auto read(const minty::net::socket&) -> type;
        static auto write(const minty::net::socket&, const type&) -> void;
    };
}
