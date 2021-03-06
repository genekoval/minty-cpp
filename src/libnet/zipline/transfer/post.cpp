#include <minty/net/zipline/transfer.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::post>::read(const n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .description = n::xfr<description_t>::read(sock),
            .date_created = n::xfr<date_created_t>::read(sock),
            .date_modified = n::xfr<date_modified_t>::read(sock),
            .objects = n::xfr<objects_t>::read(sock),
            .tags = n::xfr<tags_t>::read(sock),
            .creators = n::xfr<creators_t>::read(sock)
        };
    }

    auto n::xfr<c::post>::write(
        const n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<description_t>::write(sock, t.description);
        n::xfr<date_created_t>::write(sock, t.date_created);
        n::xfr<date_modified_t>::write(sock, t.date_modified);
        n::xfr<objects_t>::write(sock, t.objects);
        n::xfr<tags_t>::write(sock, t.tags);
        n::xfr<creators_t>::write(sock, t.creators);
    }

    auto n::xfr<n::post_parts>::read(const n::socket& sock) -> type {
        return {
            .description = n::xfr<description_t>::read(sock),
            .creators = n::xfr<creators_t>::read(sock),
            .tags = n::xfr<tags_t>::read(sock),
            .files = n::xfr<files_t>::read(sock),
            .urls = n::xfr<urls_t>::read(sock),
            .blobs = n::xfr<blobs_t>::read(sock)
        };
    }

    auto n::xfr<c::post_preview>::read(const n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .description = n::xfr<description_t>::read(sock),
            .date_created = n::xfr<date_created_t>::read(sock),
            .date_modified = n::xfr<date_modified_t>::read(sock)
        };
    }

    auto n::xfr<c::post_preview>::write(
        const n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<description_t>::write(sock, t.description);
        n::xfr<date_created_t>::write(sock, t.date_created);
        n::xfr<date_modified_t>::write(sock, t.date_modified);
    }
}
