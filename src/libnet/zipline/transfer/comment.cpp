#include <minty/net/zipline/transfer/comment.h>

namespace c = minty::core;
namespace n = minty::net;

namespace zipline {
    auto n::xfr<c::comment>::read(n::socket& sock) -> type {
        return {
            .id = n::xfr<id_t>::read(sock),
            .content = n::xfr<content_t>::read(sock),
            .indent = n::xfr<indent_t>::read(sock),
            .date_created = n::xfr<date_created_t>::read(sock)
        };
    }

    auto n::xfr<c::comment>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<id_t>::write(sock, t.id);
        n::xfr<content_t>::write(sock, t.content);
        n::xfr<indent_t>::write(sock, t.indent);
        n::xfr<date_created_t>::write(sock, t.date_created);
    }

    auto n::xfr<c::comment_node>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<c::comment>::write(sock, t.data);

        for (const auto* child : t.children) {
            n::xfr<type>::write(sock, *child);
        }
    }

    auto n::xfr<c::comment_tree>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<std::size_t>::write(sock, t.total);

        for (const auto* root : t.roots) {
            n::xfr<c::comment_node>::write(sock, *root);
        }
    }
}
