
#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::core::creator> {
        using type = minty::core::creator;

        using id_t = decltype(type::id);
        using name_t = decltype(type::name);
        using aliases_t = decltype(type::aliases);
        using bio_t = decltype(type::bio);
        using avatar_t = decltype(type::avatar);
        using banner_t = decltype(type::banner);
        using sources_t = decltype(type::sources);
        using post_count_t = decltype(type::post_count);
        using date_added_t = decltype(type::date_added);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::creator_preview> {
        using type = minty::core::creator_preview;

        using id_t = decltype(type::id);
        using name_t = decltype(type::name);
        using avatar_t = decltype(type::avatar);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };
}
