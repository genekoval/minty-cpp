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
        using posts_t = decltype(type::posts);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::object_preview> {
        using type = minty::core::object_preview;

        using id_t = decltype(type::id);
        using preview_id_t = decltype(type::preview_id);
        using mime_type_t = decltype(type::mime_type);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::data_size> {
        using type = minty::core::data_size;

        using bytes_t = decltype(type::bytes);
        using formatted_t = decltype(type::formatted);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };
}
