#include <minty/net/zipline/transfer/tag.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::tag>::read(n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .name= n::xfr<name_t>::read(sock),
            .aliases = n::xfr<aliases_t>::read(sock),
            .description = n::xfr<description_t>::read(sock),
            .avatar = n::xfr<avatar_t>::read(sock),
            .banner = n::xfr<banner_t>::read(sock),
            .sources = n::xfr<sources_t>::read(sock),
            .post_count = n::xfr<post_count_t>::read(sock),
            .date_created = n::xfr<date_created_t>::read(sock)
        };
    }

    auto n::xfr<c::tag>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<name_t>::write(sock, t.name);
        n::xfr<aliases_t>::write(sock, t.aliases);
        n::xfr<description_t>::write(sock, t.description);
        n::xfr<avatar_t>::write(sock, t.avatar);
        n::xfr<banner_t>::write(sock, t.banner);
        n::xfr<sources_t>::write(sock, t.sources);
        n::xfr<post_count_t>::write(sock, t.post_count);
        n::xfr<date_created_t>::write(sock, t.date_created);
    }

    auto n::xfr<c::tag_preview>::read(
        n::socket& sock
    ) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .name = n::xfr<name_t>::read(sock),
            .avatar = n::xfr<avatar_t>::read(sock)
        };
    }

    auto n::xfr<c::tag_preview>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<name_t>::write(sock, t.name);
        n::xfr<avatar_t>::write(sock, t.avatar);
    }
}
