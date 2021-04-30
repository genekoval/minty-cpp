#include <minty/net/zipline/transfer/source.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::source>::read(n::socket& sock) -> type {
        auto t = type();

        t.id = n::xfr<id_t>::read(sock);
        t.url = n::xfr<url_t>::read(sock);
        t.icon = n::xfr<icon_t>::read(sock);

        return t;
    }

    auto n::xfr<c::source>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<url_t>::write(sock, t.url);
        n::xfr<icon_t>::write(sock, t.icon);
    }
}
