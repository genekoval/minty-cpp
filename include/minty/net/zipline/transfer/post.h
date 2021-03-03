#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::core::post> {
        using type = minty::core::post;

        using id_t = decltype(type::id);
        using description_t = decltype(type::description);
        using date_created_t = decltype(type::date_created);
        using date_modified_t = decltype(type::date_modified);
        using objects_t = decltype(type::objects);
        using tags_t = decltype(type::tags);
        using creators_t = decltype(type::creators);

        static auto read(const minty::net::socket&) -> type;
        static auto write(const minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::post_preview> {
        using type = minty::core::post_preview;

        using id_t = decltype(type::id);
        using description_t = decltype(type::description);
        using date_created_t = decltype(type::date_created);
        using date_modified_t = decltype(type::date_modified);

        static auto read(const minty::net::socket&) -> type;
        static auto write(const minty::net::socket&, const type&) -> void;
    };
}
