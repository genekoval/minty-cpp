
#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::core::tag> {
        using type = minty::core::tag;

        using id_t = decltype(type::id);
        using name_t = decltype(type::name);
        using aliases_t = decltype(type::aliases);
        using description_t = decltype(type::description);
        using avatar_t = decltype(type::avatar);
        using banner_t = decltype(type::banner);
        using sources_t = decltype(type::sources);
        using post_count_t = decltype(type::post_count);
        using date_created_t = decltype(type::date_created);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::tag_name> {
        using type = minty::core::tag_name;

        using name_t = decltype(type::name);
        using aliases_t = decltype(type::aliases);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::tag_preview> {
        using type = minty::core::tag_preview;

        using id_t = decltype(type::id);
        using name_t = decltype(type::name);
        using avatar_t = decltype(type::avatar);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };
}
