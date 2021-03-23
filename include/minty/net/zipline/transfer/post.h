#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/post_parts.h>
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
        using creators_t = decltype(type::creators);

        static auto read(minty::net::socket&) -> type;
        static auto write(minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::net::post_parts> {
        using type = minty::net::post_parts;

        using title_t = decltype(type::title);
        using description_t = decltype(type::description);
        using creators_t = decltype(type::creators);
        using tags_t = decltype(type::tags);
        using files_t = decltype(type::files);
        using urls_t = decltype(type::urls);
        using blobs_t = decltype(type::blobs);

        static auto read(minty::net::socket&) -> type;
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
}
