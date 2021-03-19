#include <minty/net/zipline/transfer.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::tag>::read(n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .name = n::xfr<name_t>::read(sock),
            .color = n::xfr<color_t>::read(sock),
            .date_created = n::xfr<date_created_t>::read(sock)
        };
    }

    auto n::xfr<c::tag>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<name_t>::write(sock, t.name);
        n::xfr<color_t>::write(sock, t.color);
        n::xfr<date_created_t>::write(sock, t.date_created);
    }
}
