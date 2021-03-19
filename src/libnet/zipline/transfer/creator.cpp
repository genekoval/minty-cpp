#include <minty/net/zipline/transfer.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::creator>::read(n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .name= n::xfr<name_t>::read(sock),
            .aliases = n::xfr<aliases_t>::read(sock),
            .bio = n::xfr<bio_t>::read(sock),
            .avatar = n::xfr<avatar_t>::read(sock),
            .banner = n::xfr<banner_t>::read(sock),
            .sources = n::xfr<sources_t>::read(sock),
            .post_count = n::xfr<post_count_t>::read(sock),
            .date_added = n::xfr<date_added_t>::read(sock)
        };
    }

    auto n::xfr<c::creator>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<name_t>::write(sock, t.name);
        n::xfr<aliases_t>::write(sock, t.aliases);
        n::xfr<bio_t>::write(sock, t.bio);
        n::xfr<avatar_t>::write(sock, t.avatar);
        n::xfr<banner_t>::write(sock, t.banner);
        n::xfr<sources_t>::write(sock, t.sources);
        n::xfr<post_count_t>::write(sock, t.post_count);
        n::xfr<date_added_t>::write(sock, t.date_added);
    }

    auto n::xfr<c::creator_preview>::read(
        n::socket& sock
    ) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .name = n::xfr<name_t>::read(sock),
            .avatar = n::xfr<avatar_t>::read(sock)
        };
    }

    auto n::xfr<c::creator_preview>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<name_t>::write(sock, t.name);
        n::xfr<avatar_t>::write(sock, t.avatar);
    }
}
