#pragma once

#include <minty/net/zipline/protocol.h>
#include <minty/server/server_info.h>

namespace zipline {
    template <>
    struct transfer<minty::net::socket, minty::server::server_info> {
        using type = minty::server::server_info;

        using object_source_t = decltype(type::object_source);
        using version_t = decltype(type::version);

        static auto read(const minty::net::socket&) -> type;
        static auto write(const minty::net::socket&, const type&) -> void;
    };
}
