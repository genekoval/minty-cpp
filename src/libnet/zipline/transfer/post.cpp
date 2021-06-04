#include <minty/net/zipline/transfer/object.h>
#include <minty/net/zipline/transfer/post.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::post>::read(n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .title = n::xfr<title_t>::read(sock),
            .description = n::xfr<description_t>::read(sock),
            .date_created = n::xfr<date_created_t>::read(sock),
            .date_modified = n::xfr<date_modified_t>::read(sock),
            .objects = n::xfr<objects_t>::read(sock),
            .tags = n::xfr<tags_t>::read(sock)
        };
    }

    auto n::xfr<c::post>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<title_t>::write(sock, t.title);
        n::xfr<description_t>::write(sock, t.description);
        n::xfr<date_created_t>::write(sock, t.date_created);
        n::xfr<date_modified_t>::write(sock, t.date_modified);
        n::xfr<objects_t>::write(sock, t.objects);
        n::xfr<tags_t>::write(sock, t.tags);
    }

    auto n::xfr<c::post_parts>::read(n::socket& sock) -> type {
        return {
            .title = n::xfr<title_t>::read(sock),
            .description = n::xfr<description_t>::read(sock),
            .objects = n::xfr<objects_t>::read(sock),
            .tags = n::xfr<tags_t>::read(sock)
        };
    }

    auto n::xfr<c::post_parts>::write(n::socket& sock, const type& t) -> void {
        n::xfr<title_t>::write(sock, t.title);
        n::xfr<description_t>::write(sock, t.description);
        n::xfr<objects_t>::write(sock, t.objects);
        n::xfr<tags_t>::write(sock, t.tags);
    }

    auto n::xfr<c::post_preview>::read(n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .title = n::xfr<title_t>::read(sock),
            .preview_id = n::xfr<preview_id_t>::read(sock),
            .comment_count = n::xfr<comment_count_t>::read(sock),
            .object_count = n::xfr<object_count_t>::read(sock),
            .date_created = n::xfr<date_created_t>::read(sock)
        };
    }

    auto n::xfr<c::post_preview>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<title_t>::write(sock, t.title);
        n::xfr<preview_id_t>::write(sock, t.preview_id);
        n::xfr<comment_count_t>::write(sock, t.comment_count);
        n::xfr<object_count_t>::write(sock, t.object_count);
        n::xfr<date_created_t>::write(sock, t.date_created);
    }
}
