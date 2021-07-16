#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::core::post> {
        using type = minty::core::post;

        using id_t = decltype(type::id);
        using title_t = decltype(type::title);
        using description_t = decltype(type::description);
        using date_created_t = decltype(type::date_created);
        using date_modified_t = decltype(type::date_modified);
        using objects_t = decltype(type::objects);
        using tags_t = decltype(type::tags);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::post_parts> {
        using type = minty::core::post_parts;

        using title_t = decltype(type::title);
        using description_t = decltype(type::description);
        using objects_t = decltype(type::objects);
        using tags_t = decltype(type::tags);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::post_preview> {
        using type = minty::core::post_preview;

        using id_t = decltype(type::id);
        using title_t = decltype(type::title);
        using preview_id_t = decltype(type::preview_id);
        using comment_count_t = decltype(type::comment_count);
        using object_count_t = decltype(type::object_count);
        using date_created_t = decltype(type::date_created);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };

    ZIPLINE_OBJECT(
        minty::core::post_search,
        &minty::core::post_search::id,
        &minty::core::post_search::title,
        &minty::core::post_search::description,
        &minty::core::post_search::date_created,
        &minty::core::post_search::date_modified,
        &minty::core::post_search::tags
    );

    template <>
    struct transfer<minty::net::socket, minty::core::range> {
        using type = minty::core::range;

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };
}
