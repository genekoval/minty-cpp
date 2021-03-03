#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::core::comment> {
        using type = minty::core::comment;

        using id_t = decltype(type::id);
        using content_t = decltype(type::content);
        using indent_t = decltype(type::indent);
        using date_created_t = decltype(type::date_created);

        static auto read(const minty::net::socket&) -> type;
        static auto write(const minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::comment_node> {
        using type = minty::core::comment_node;

        static auto write(const minty::net::socket&, const type&) -> void;
    };

    template <>
    struct transfer<minty::net::socket, minty::core::comment_tree> {
        using type = minty::core::comment_tree;

        static auto write(const minty::net::socket&, const type&) -> void;
    };

}
