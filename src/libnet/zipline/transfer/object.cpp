#include <minty/net/zipline/transfer.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::object>::read(const n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .hash = n::xfr<hash_t>::read(sock),
            .size = n::xfr<size_t>::read(sock),
            .mime_type = n::xfr<mime_type_t>::read(sock),
            .date_added = n::xfr<date_added_t>::read(sock),
            .preview_id = n::xfr<preview_id_t>::read(sock),
            .src = n::xfr<src_t>::read(sock)
        };
    }

    auto n::xfr<c::object>::write(
        const n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<hash_t>::write(sock, t.hash);
        n::xfr<size_t>::write(sock, t.size);
        n::xfr<mime_type_t>::write(sock, t.mime_type);
        n::xfr<date_added_t>::write(sock, t.date_added);
        n::xfr<preview_id_t>::write(sock, t.preview_id);
        n::xfr<src_t>::write(sock, t.src);
    }
}
