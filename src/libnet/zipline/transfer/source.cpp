#include <minty/net/zipline/transfer.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::site>::read(const n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .name = n::xfr<name_t>::read(sock),
            .homepage = n::xfr<homepage_t>::read(sock),
            .thumbnail_id = n::xfr<thumbnail_id_t>::read(sock)
        };
    }

    auto n::xfr<c::site>::write(
        const n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<name_t>::write(sock, t.name);
        n::xfr<homepage_t>::write(sock, t.homepage);
        n::xfr<thumbnail_id_t>::write(sock, t.thumbnail_id);
    }

    auto n::xfr<c::source>::read(const n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .url = n::xfr<url_t>::read(sock),
            .website = n::xfr<website_t>::read(sock)
        };
    }

    auto n::xfr<c::source>::write(
        const n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<url_t>::write(sock, t.url);
        n::xfr<website_t>::write(sock, t.website);
    }
}
