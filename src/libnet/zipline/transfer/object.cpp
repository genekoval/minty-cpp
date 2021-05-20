#include <minty/net/zipline/transfer/object.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::object>::read(n::socket& sock) -> type {
        auto t = type();

        t.id = n::xfr<id_t>::read(sock);
        t.hash = n::xfr<hash_t>::read(sock);
        t.size = n::xfr<size_t>::read(sock);
        t.mime_type = n::xfr<mime_type_t>::read(sock);
        t.date_added = n::xfr<date_added_t>::read(sock);
        t.preview_id = n::xfr<preview_id_t>::read(sock);
        t.src = n::xfr<src_t>::read(sock);
        t.posts = n::xfr<posts_t>::read(sock);

        return t;
    }

    auto n::xfr<c::object>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<hash_t>::write(sock, t.hash);
        n::xfr<size_t>::write(sock, t.size);
        n::xfr<mime_type_t>::write(sock, t.mime_type);
        n::xfr<date_added_t>::write(sock, t.date_added);
        n::xfr<preview_id_t>::write(sock, t.preview_id);
        n::xfr<src_t>::write(sock, t.src);
        n::xfr<posts_t>::write(sock, t.posts);
    }

    auto n::xfr<c::object_preview>::read(n::socket& sock) -> type {
        auto t = type();

        t.id = n::xfr<id_t>::read(sock);
        t.preview_id = n::xfr<preview_id_t>::read(sock);
        t.mime_type = n::xfr<mime_type_t>::read(sock);

        return t;
    }

    auto n::xfr<c::object_preview>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<preview_id_t>::write(sock, t.preview_id);
        n::xfr<mime_type_t>::write(sock, t.mime_type);
    }

    auto n::xfr<c::data_size>::read(n::socket& sock) -> type {
        auto t = type();

        t.bytes = n::xfr<bytes_t>::read(sock);
        t.formatted = n::xfr<formatted_t>::read(sock);

        return t;
    }

    auto n::xfr<c::data_size>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<bytes_t>::write(sock, t.bytes);
        n::xfr<formatted_t>::write(sock, t.formatted);
    }
}
